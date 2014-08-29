#include <SoftwareSerial.h>
#include <WunderbarBridge.h>

#define BRIDGE_DEBUG true

const int DEBUG_TX = 11;
const int DEBUG_RX = 10;

Bridge bridge = Bridge(DEBUG_RX, DEBUG_TX, 115200); 

int led = 7;
int button = 2;
int vibrator = 4;

void setup() {
	pinMode(led, OUTPUT);
	pinMode(vibrator, OUTPUT);
	pinMode(button, INPUT);
	  
	bridge.begin();
}

void loop() {
	static uint8_t dataOut[2] = {1, 2};

	/* If new data is received blink the LED */
	if (bridge.newData == true){
		bridge.newData = false;

		digitalWrite(led, HIGH);
		digitalWrite(vibrator, HIGH);
    	delay(2000);
    	digitalWrite(vibrator, LOW);
    	digitalWrite(led, LOW);
	}

	
	/* On a button press Send a fixed test packet */
	if (digitalRead(button))
	{
		bridge.sendData(dataOut, 2);

		digitalWrite(led, HIGH);
		delay(5000);
		digitalWrite(led, LOW);
	}

}

void serialEvent(){
	bridge.processSerial();
}




