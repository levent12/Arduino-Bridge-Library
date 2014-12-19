/* 
    *** Simple example that shows how to use the Wunderbar Bridge module
    *** to connect an Ardino to the relayr open sensor cloud.

    The application uses Serial communication to send data from the serial monitor to the Arduino then to the bridge module and finally to the relayr open sensor cloud,
    and receivs data from the cloud back to the bridge to the Arduino again. You can see the data sent/received through the serial monitor and the developer dashboard.
 */
 
#include <WunderbarBridge.h>

/*  Config the bridge module with the baudrate,
    which has to be the same used in the PC serial monitor application*/
	
Bridge bridge = Bridge(115200);
static bridge_payload_t rxPayload;
static uint8_t dataout[1];

void setup() {	  
  if(bridge.begin())
  Serial.print("Bridge Connected\n");
  else
  Serial.print("Bridge NotConnected\n");
  
  //Set the port for debugging, default is Serial0
  //bridge.setDebugPort(0);
  //Set the port for bridge connection, default is Serial1
  //bridge.setBridgePort(1);
}

/* Main Loop */
void loop() {
  //when we get new data from the cloud
  if (bridge.newData){		
    Serial.println("Data Received!\n");  
    rxPayload = bridge.getData();	
    delay(100);
  }
  
  //enter data in Serial monitor and send it to bridge
  if(Serial.available())
  {
     char c = Serial.read();
     dataout[0] = c;
     bridge.sendData(dataout,sizeof(dataout));
  }
  
  if(Serial1.available())
  {
     bridge.processSerial();
  }
}