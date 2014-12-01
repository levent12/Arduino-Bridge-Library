/* 
    *** Simple example that shows how to use the Wunderbar Bridge module
    *** to connect an Ardino to the relayr open sensor cloud.

    The application uses Serial communication to send data from the Arduino to the bridge module then to the relayr open sensor cloud,
    and receivs data from the cloud back to the bridge to the Arduino again. You can see the data sent/received through the debug serial monitor.
 */
 
#include <WunderbarBridge.h>
#include <SoftwareSerial.h>

/*  Config the bridge module with the baudrate,
    which has to be the same used in the PC serial monitor application*/
Bridge bridge = Bridge(115200);
static bridge_payload_t rxPayload;
static uint8_t dataout[2] = {1, 2};
  
void setup() {	  
  if(bridge.begin())
  Serial.print("Bridge Connected\n");
  else
  Serial.print("Bridge NotConnected\n");
  
  //Set the port for debugging, default is 0
  //bridge.setDebugPort(0);
  //Set the port for bridge connection, default is 1
  //bridge.setBridgePort(1);
}

/* Main Loop */
void loop() {
  //when we get new data from the cloud
  if (bridge.newData == true){		
    Serial.println("Data Received!\n");  
    rxPayload = bridge.getData();	
    delay(100);
  }
  //enter data in Serial monitor and send it to bridge
  if(Serial.available())
  {
     char c =Serial.read();
     Serial.println(c);
     static uint8_t dataout2[1];
     dataout2[0]=c;
//     Serial1.write(c);
     bridge.sendData(dataout2,sizeof(dataout2));
  }
  if(Serial1.available())
  {
     bridge.processSerial();
  }
}

// Same as Serial1.available() 
/*
void serialevent1()
{
  Serial.print("hi");
  bridge.processSerial();
}*/




