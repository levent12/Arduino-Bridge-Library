#include <SoftwareSerial.h>
#include <WunderbarBridge.h>

#define BRIDGE_DEBUG true

const int DEBUG_TX = 11;
const int DEBUG_RX = 10;

Bridge bridge = Bridge(DEBUG_RX, DEBUG_TX, 115200); 

int led = 7;
int button = 2;

void setup() {
	pinMode(led, OUTPUT);
	pinMode(button, INPUT);
	  
	bridge.begin();
}

void loop() {
	static uint8_t dataOut[2] = {1, 2};

	if (bridge.newData == true){
		bridge.newData = false;

		digitalWrite(led, HIGH);
    	delay(2000);
    	digitalWrite(led, LOW);
	}

	if (!digitalRead(button))
	{
		//if (bridge.checkConnection())
			bridge.sendData(dataOut, 2);

		digitalWrite(led, HIGH);
		delay(1000);
		digitalWrite(led, LOW);
	}

}

void serialEvent(){
	bridge.processSerial();
}




