#include <SoftwareSerial.h>
#include <WunderbarBridge.h>
#include <LED_Bar.h>

#define BRIDGE_DEBUG true

const int DEBUG_TX = 11;
const int DEBUG_RX = 10;

const int LED_BAR_CLK = 13;
const int LED_BAR_DI = 12;

Bridge bridge = Bridge(DEBUG_RX, DEBUG_TX, 115200); 

LED_Bar bar(LED_BAR_CLK, LED_BAR_DI);   // config Io here, (clk, dio)

const int led = 7;
const int button = 2;
const int vibrator = 4;

void setup() {
	pinMode(led, OUTPUT);
	pinMode(vibrator, OUTPUT);
	pinMode(button, INPUT);
	  
	bridge.begin();

	bar.setLevel(0);
}

void loop() {
	 
	static uint8_t dataOut[2] = {1, 2};
	static bridge_payload_t rxPayload;

	if (bridge.newData == true){
		
		rxPayload = bridge.getData();
	
		digitalWrite(led, HIGH);
		digitalWrite(vibrator, HIGH);

		//Use the payload[1] to set the LED_BAR level
		bar.setLevel(rxPayload.payload[1]);	

    	delay(1000);

    	digitalWrite(vibrator, LOW);
    	digitalWrite(led, LOW);
	}

	
	/* On a button press Send a fixed test packet */
	if (digitalRead(button))
	{
		bridge.sendData(dataOut, 2);

		digitalWrite(led, HIGH);
		delay(2000);
		digitalWrite(led, LOW);
	}

}

void serialEvent(){
	bridge.processSerial();
}




