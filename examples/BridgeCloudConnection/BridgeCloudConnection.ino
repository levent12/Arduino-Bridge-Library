  /*
  	*** This example only works with the Arduino Mega/Due boards & the Arduino Mega library as it uses multiple serial communication.
  	One serial communication is opened at Serial0 through the USB to allow communication between Arduino IDE and the board, while the other	is used to communicate with the bridge.
  
          *** Simple example that shows how to use the Wunderbar Bridge module to connect an Arduino program to the relayr open sensor cloud.
          The application uses Serial communication to send data from your laptop through the serial monitor to an Arduino board which then sends it to the bridge module which will finally 
  	publish the data to the relayr open sensor cloud, where you can access it through the relayr API or SDKs. It can also receive data from the cloud back to the bridge. 
          You can see the data sent/received through your serial monitor and the developer dashboard.
   */
   
  #include <WunderbarBridge.h>
  
  /*  Config the bridge module with the baudrate which has to be the same used in the PC serial monitor application.
  	Assumes bridge is connected to Serial1 by default.
  */
  	
  Bridge bridge = Bridge(115200);
  static bridge_payload_t rxPayload;
  static uint8_t dataout[1];
  
  void setup() {	  
    
    if(bridge.begin())
    Serial.print("Bridge Connected\n");
    else
    Serial.print("Bridge Not Connected\n");
    
    //Change the port for debugging, default is Serial0 (one used for serial monitor)
    //bridge.setDebugPort(0);
    //Change the Serial port of the bridge connection, default is Serial1
    //bridge.setBridgePort(2);
  }
  
  /* Main Loop */
  void loop() {
    //when we receive new data from the cloud on the down channel
    if (bridge.newData){		
      Serial.println("Data Received!\n");  
      rxPayload = bridge.getData();	
      delay(100);
    }
    
    //On receiving data from the Serial monitor, send it to the cloud
    if(Serial.available())
    {
       char c = Serial.read();
       dataout[0] = c;
       bridge.sendData(dataout,sizeof(dataout));
    }
    
    //Updates the bridge module with the data it receives on the UART
    if(Serial1.available())
    {
       bridge.processSerial();
    }
  }
