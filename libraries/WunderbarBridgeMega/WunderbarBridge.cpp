/* Library for connect Arduino to the Wunderbar Bridge Module
** Author: Daniel Mancuso & Khaled Osman
** Relayr.io
*/

#include "Arduino.h"
#include "WunderbarBridge.h"

//define default Serial ports
HardwareSerial* bridgeSerial=&Serial1;
HardwareSerial* debugSerial=&Serial;

Bridge::Bridge(){
      useDebugOutput = false;
}

Bridge::Bridge(int32_t baudrate){
	  _baudrate=baudrate;
	  useDebugOutput=true;
}

//------------------------------------------------------------------------------
/**
 *    Initialize Serial port and print welcome message
 *
 * \param
 *
 * \return True if a Bridge Connection was detected
 *         False if no answer received from the Bridge
 */
bool Bridge::begin()
{
  /* 115200 is the default baudrate for the Bridge
    (could be changed in the BLE Config char)*/
  bridgeSerial->begin(115200);

  if (useDebugOutput){
    //initialize Debug Serial UART:
    debugSerial->begin(_baudrate);
  	debugSerial->println("\n\r\n\r------------------------------- \
                          \n\r relayr - bring things to life  \
                          \n\r Arduino / WunderBar-Bridge lib \
                          \n\r-------------------------------\n\r");
  }
  return checkConnection();
}

//------------------------------------------------------------------------------
/**
 * Sets the Hardware Serial port the Bridge is Connected to
 *
 * \param: The number of the hardware serial the bridge is connected to; can be 0,1,2 or 3
 *
 * \return
 */
void Bridge::setBridgePort(uint8_t portNumber){
	switch(portNumber)
	{
		//use RX0, TX0
		case 0:
		bridgeSerial = &Serial;
		break;

		//use RX1, TX1
		case 1:
		bridgeSerial = &Serial1;
		break;

		//use RX2, TX2
		case 2:
		bridgeSerial = &Serial2;
		break;

		//use RX3, TX3
		case 3:
		bridgeSerial = &Serial3;
		break;

		//wrong input
		default:
		debugSerial->println("Bridge Port not changed, port number must be a number between 0 and 3");
		break;
	}
}

/**
 * Sets the Hardware Serial port used for debugging
 *
 * \param: the hardware serial used for debugging; can be 0,1,2 or 3
 *
 * \return
 */
void Bridge::setDebugPort(uint8_t portNumber){
	switch(portNumber)
	{
		//use RX0, TX0
		case 0:
		debugSerial = &Serial;
		break;

		//use RX1, TX1
		case 1:
		debugSerial = &Serial1;
		break;

		//use RX2, TX2
		case 2:
		debugSerial = &Serial2;
		break;

		//use RX3, TX3
		case 3:
		debugSerial = &Serial3;
		break;

		//wrong input
		default:
		debugSerial->println("Debug Port not changed, port number must be a number between 0 and 3");
		break;
	}
}

//------------------------------------------------------------------------------
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void Bridge::processSerial(void)
{
  //debugSerial->println("processSerial");//******************debug
  while (bridgeSerial->available())
  {
    // get the new byte:
    uint8_t inChar = (uint8_t) bridgeSerial->read();

    //debugSerial->println(inChar);//******************debug

    down.rec_bytes++;

    if (down.rec_bytes == 1) //1: command
    {
      down.channel.command = inChar;

      switch (inChar)
      {
          case BRIDGE_COMM_ACK:
            down.rec_bytes = 0;
            commandReceived = true;
            break;

          case BRIDGE_COMM_NACK:
            down.rec_bytes = 0;
            commandReceived = true;
            break;

          case BRIDGE_COMM_NCONN:
            down.rec_bytes = 0;
            commandReceived = true;

            if (useDebugOutput){
              debugSerial->println("\n\n\rError: Bridge Module not connected to the Wunderbar");
            }
            break;

          case BRIDGE_COMM_PING:
            bridgeSerial->write(BRIDGE_COMM_ACK);
            down.rec_bytes = 0;
            commandReceived = true;
            break;

          case BRIDGE_COMM_RCV_FROM_BLE:
            break;

          default:
            down.rec_bytes = 0;
            down.packet_ok = false;
      }
    }

    if (down.rec_bytes == 2)
        down.channel.length = inChar; //2: Length

    if ((down.rec_bytes > 2) && (down.rec_bytes <= (down.channel.length + 2)))  //3:Start Payload
    {
      down.channel.payload[down.payload_c] = inChar;
      down.payload_c++;
    }

    if (down.rec_bytes == down.channel.length + 3)
        down.channel.crc = inChar; //CRC16 LSB

    if (down.rec_bytes == down.channel.length + 4)  //CRC16 MSB
    {
      down.channel.crc += (inChar << 8);

      uint16_t c_crc = crc16Compute((uint8_t *) &down.channel, down.channel.length+2, NULL);

      if (c_crc == down.channel.crc)
      {
        down.packet_ok = true;
      } else
            down.packet_ok = false;

      commandReceived = true;
      newData = true;

      //reset counters
      down.rec_bytes = 0;
      down.payload_c = 0;

      if (useDebugOutput){
        debugSerial->println("\n\n\r<<<== Packet received:");

        if (down.packet_ok)
            debugSerial->print("CRC OK");

        dumpPacket(down.channel);
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
 * Sends a packet over the Hardware Serial port (TX)
 *
 * \param: byte array with the payload and payload size
 *
 * \return
 */
bool Bridge::sendData(uint8_t payload[], int size)
{
  //prepares the packet
  bridge_comm_t outPacket = createUpPacket(payload, size, outputBuffer);

  if (!_bridgeConnected){
    if (!checkConnection()) return false;
  }

   // memcpy((char *) &outPacket, outputBuffer, sizeof(outPacket));//outPacket.length + 4);
  if (useDebugOutput){
    debugSerial->println("\n\r==>>> OutPacket:");
    dumpPacket(outPacket);

    debugSerial->print("output Buffer: ");
    for (char i=0; i < outPacket.length+4; i++)
    {
      debugSerial->print(outputBuffer[i], HEX);
      debugSerial->print(",");
    }
  }
  //sends the packet to the UART
  bridgeSerial->write((uint8_t *)outputBuffer, outPacket.length + 4);

  return true;
}

//------------------------------------------------------------------------------
/**
 *    Calculate a CRC (16 bits) of a packet
 *
 * \params: pointer to the data, data size, initialization word (NULL by default)
 *
 * \return: the calculated CRC
 */
uint16_t Bridge::crc16Compute(uint8_t * p_data, int size, uint8_t * p_crc)
{
  uint32_t i;
  uint16_t crc = (p_crc == NULL) ? 0xffff : *p_crc;

  for (i = 0; i < size; i++)
  {
    crc = (uint8_t)(crc >> 8) | (crc << 8);
    crc ^= p_data[i];
    crc ^= (uint8_t)(crc & 0xff) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xff) << 4) << 1;
  }
  return crc;
}

//------------------------------------------------------------------------------
/**
 *   Creates an Up Data Packet
 *
 * \params: pointer to the payload, payload length, pointer to the output buffer.
 *
 * \return: the bridge_comm object
 */
bridge_comm_t Bridge::createUpPacket(uint8_t * payload, int length, uint8_t * outBuffer)
{
  bridge_comm_t packet;

  packet.command = BRIDGE_COMM_WRITE_UP_CHANNEL;
  packet.length = length;

  memcpy(packet.payload, payload, length);

  outBuffer[0] = packet.command;
  outBuffer[1] = packet.length;

  for (char i = 2; i < length + 2; i++)
  {
    outBuffer[i] = packet.payload[i-2];
  }

  packet.crc = crc16Compute(outBuffer, length + 2, NULL);

  outBuffer[length + 2] = (packet.crc & 0xff);
  outBuffer[length + 3] = (packet.crc >> 8);

  return packet;
}

//------------------------------------------------------------------------------
/**
 *   Returns a struct with received payload
 * \param: A pointer to the array where to put the data
 *
 * \return none
 */
bridge_payload_t Bridge::getData(void)
{
  bridge_payload_t rx_payload;

  memcpy(rx_payload.payload, down.channel.payload, down.channel.length);
  rx_payload.length = down.channel.length;
  newData = false;

  return rx_payload;
}

//------------------------------------------------------------------------------
/**
 *   Print the details of a packet in the Debug Serial port
 * \param: A bridge_comm Packet
 *
 * \return none
 */
void Bridge::dumpPacket(bridge_comm_t packet)
{
  if (useDebugOutput){
    debugSerial->print("\n\r-----------\n\r");
    debugSerial->print("Packet size: ");
    debugSerial->println(packet.length+4);
    debugSerial->print("CMD: ");
    debugSerial->println(packet.command, HEX);
    debugSerial->print("Payload length: ");
    debugSerial->println(packet.length, DEC);
    debugSerial->print("Payload: ");

    for (char i = 2; i < packet.length+2; i++)
    {
      debugSerial->print(packet.payload[i-2], HEX);
      debugSerial->print(", ");
    }

    debugSerial->print("\n\rCRC: ");
    debugSerial->print(packet.crc, HEX);
    debugSerial->print("\n\r-----------\n\r");
  }
}

//------------------------------------------------------------------------------
/**
 * Sends a PING command to the Bridge and wait for the response
 *
 * \param
 *
 * \return True if gets the ACK from the Bridge
 *         False if no command received
 */
bool Bridge::checkConnection()
{
  bridgeSerial->write(BRIDGE_COMM_PING);

  for (uint8_t i = 0; i < 10; ++i)
  {
    if (bridgeSerial->available()){
      processSerial();
      break;
    }
    delay(10);
  }

  if (!commandReceived || (down.channel.command != BRIDGE_COMM_ACK))
  {
    if (useDebugOutput){
      debugSerial->println("\n\n\r *** Could not find a valid Bridge module, check wiring and that the Brige has the UART firmware.\n\r\
       Arduino TX ==>> Bridge RX (white wire)\n\r\
       Arduino RX ==>> Bridge TX (Yellow wire)");
    }
   _bridgeConnected = false;
   return false;
  }
  _bridgeConnected = true;
  return true;
}
