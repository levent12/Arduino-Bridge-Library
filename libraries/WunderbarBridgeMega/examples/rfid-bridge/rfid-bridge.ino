//This example reads tags from RFID module and send its readings to the bridge module.
//then publishes it to the cloud, hardware serial is used for debugging

#include <WunderbarBridge.h>
#include <SoftwareSerial.h>

SoftwareSerial SoftSerial(10, 11); // define our software serial that communicates with RFID
char buffer[64]; // buffer array for data receive over serial port
int count = 0;   // counter for buffer array

Bridge bridge = Bridge(115200);
static bridge_payload_t rxPayload;
static uint8_t dataout[7];

void setup()
{
  SoftSerial.begin(9600);               // the SoftSerial baud rate
  bridge.setBridgePort(3); // use RX3, TX3 for bridge connection
  if (bridge.begin())
    Serial.print("Bridge Connected\n");
  else
    Serial.print("Bridge Not Connected\n");
}

void loop()
{
  if (SoftSerial.available())              // if date is coming from software serial port ==> data is coming from SoftSerial shield
  {
    while (SoftSerial.available())         // reading data into char array
    {
      buffer[count++] = SoftSerial.read();   // writing data into array
      if (count == 64) {
        break;
      }
    }

    //copy the first 7 bytes from our rfid reading to our dataout array
    memcpy(dataout, buffer, 7);
    Serial.write(buffer, sizeof(buffer));           // if no data transmission ends, write buffer to hardware serial port
    //send data to the cloud
    bridge.sendData(dataout, sizeof(dataout));

    clearBufferArray();              // call clearBufferArray function to clear the stored data from the array
    count = 0;                       // set counter of while loop to zero
  }

  if (Serial.available())            // if data is available on hardware serial port ==> data is coming from PC or notebook
    SoftSerial.write(Serial.read());       // write it to the SoftSerial shield

  //on receiving command from cloud to bridge
  if (bridge.newData) {
    Serial.println("Data Received!\n");
    rxPayload = bridge.getData();
    delay(100);
  }
  //communication loop for bridge
  if (Serial3.available())
  {
    bridge.processSerial();
  }
}
void clearBufferArray()              // function to clear buffer array
{
  for (int i = 0; i < count; i++)
  {
    buffer[i] = NULL; // clear all index of array with command NULL
  }
}


