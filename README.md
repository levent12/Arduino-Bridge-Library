The Arduino-Bridge Library
------------
Welcome to the **Bridge library for Arduino**.
The components in this repository allow you to connect the Wunderbar Bridge/Grove module to the [Arduino UART](http://arduino.cc/) and in just 10 minutes have an easy gateway to the **relayr Cloud Platform**

This repository contains the [WunderBarBridgeMega](https://github.com/relayr/Arduino-Bridge-Library/tree/master/libraries/WunderbarBridgeMega) for Arduino boards with multiple serial ports.

Boards such as the **[Arduino Mega](http://arduino.cc/en/Main/arduinoBoardMega)** have multiple hardware serial ports. four ports in total. You can set your debug/bridge connection port to be any of the 4 ports. The same goes for the bridge module. You are able to see the data sent and received via the Arduino default serial monitor 
when the debug port is set to 0, without the need for an external application. 

![Picture of the example](assets/arduinoBridge.gif)

## Getting Started

### Prerequisites

<ol>
        <li>A Wunderbar-Bridge connected to a power supply</li>
        <li>An Arduino (we recommend the Arduino MEGA for debugging purposes)</li>
        <li><a href="http://www.seeedstudio.com/depot/grove-branch-cable-for-servo5pcs-pack-p-753.html?cPath=178_179">This type of cabel</a></li>
        <li>A smartphone</li>
        <li><a href="https://www.arduino.cc/en/Main/Software">the Arduino Software (IDE)</a></li>
        <li>
            <a href="https://github.com/relayr/Arduino-Bridge-Library">The Arduino-Bridge-Library</a></li>
        <li>The Manager app <a href="https://play.google.com/store/apps/details?id=io.relayr.wunderbar">either for android</a> or <a href="https://itunes.apple.com/de/app/wunderbar-onboarding/id909224330?mt=8">for iOS</a></li>
    </ol>
</p>
<p>

Below is a step by step tutorial of how to install and use the repository as well as a sketch example to get you inspired before you start your own project.

<p>
    <h3>Installing the Library</h3>
    <ol>
        <li>Download the repository as a .zip file unzip it.</li>
        <li>Copy the folder <i>WunderbarBridgeMega</i> to <i>Arduino</i> libraries folder (on Mac: Documents/Arduino/libraries) (this is the Manual installation)</li>
    </ol>
</p>

----------

<p>
    <h3>Flashing the UART firmware on the Bridge Module</h3>
    <span>In order to allow the Bridge Module to communicate with the Arduino over UART, it is necessary to flash the Bridge UART Firmware.</span>
    <ol>
        <li>Copy the "sensor_bridge_fw_UART.hex" form /Arduino/libraries/WunderbarBridgeMega onto your Phone</li>
        <li>Start the Manager app, select the GroveConnector and activate the BLE direct Connection mode(To do so simply swipe to the left and activate BLE mode)</li>
        <li>Select 'Update Firmware' and choose the option to 'select from folder'</li>
        <li>Place the Bridge Modul in <i>firmware update mode</i>. You can do this by pressing the button on the module and holding it for 3 seconds. The LED will be constantly on when <i>firmware update mode</i>is activated.</li>
        <li>Follow the instructions in the app to flash the firmware</li>
    </ol>
</p>



----------

### Connecting  the Bridge Module to Arduino

The easiest way of connecting the two is using the [*"Grove Servo cable"*](http://www.seeedstudio.com/depot/grove-branch-cable-for-servo5pcs-pack-p-753.html?cPath=178_179) from Seedstudio. Below is the list of connections defined by Pin number / Name:


**Bridge:** --------- **Arduino:** 

<strong><ol>
        <li>Tx ----------> RX1 - Rx</li>
        <li>Rx ----------> TX1 - Tx</li>
        <li>VEXT ---------> 5 Volts</li>
        <li>GND ---------> GND</li>
</ol></strong>

----------

### Debugging Connection to the PC Serial Port 

With the ArduinoMega you could use any port for debugging. The default debugging port in 0 as to allow you to debug via your computer:


**Arduino:** ------ **PC Serial port:**

*  **0 - Rx** --------> **Tx**
* **1 - Tx** --------> **Rx**
* **GND** -----------> **GND**

----------

### Viewing The URAT Output

in order to view the output for the ArduinoMega you could use the Serial Monitor from your Arduino without the need for an external software.


## Using the library
#### Above the setup:
Include the WunderbarBridge header and instantiate the Bridge class to use: 
<code>
	#include <WunderbarBridge.h> 
	Bridge bridge = Bridge(115200);
</code>

#### In the setup:
<code>
	bridge.setDebugPort(0); 
	bridge.setBridgePort(1); 
	bridge.begin();
</code>

#### In the loop:
<code>
	if (Serial1.available()) {
	bridge.processSerial();
	}
	bridge_payload_t rxPayload = bridge.getData();
	
	uint8_t dataOut[] = {1, 2, 3};
	bridge.sendData(dataOut, sizeof(dataOut));
<code>

#### Your code should look something like this:
<code>
	#include <WunderbarBridge.h>
	Bridge bridge = Bridge(115200);
	
	void setup() {
	bridge.setDebugPort(0);
	bridge.setBridgePort(1);
	bridge.begin();
	}br 
	void loop() { 
	if (Serial1.available()) {
	bridge.processSerial();
	}
	bridge_payload_t rxPayload = bridge.getData();
	
	uint8_t dataOut[] = {1, 2, 3};
	bridge.sendData(dataOut, sizeof(dataOut));
	}
</code>

Next,	
	    	
   1. Under Tools select 'Board your Arduino MEGA'
   2. Uplaod your code onto the Arduino
   3. Open the <i>Serial Monitor</i> and change it to <i>115200 baud

To get a bit more insight into using the library, have a look at the [Cloud Connection Example ](https://github.com/relayr/Arduino-Bridge-Library/blob/master/examples/BridgeCloudConnection/BridgeCloudConnection.ino)


## Testing the connection between the Bridge Module and the Master Module:

1. The Bridge Module has to be onboarded again after a [firmware change](https://developer.relayr.io/documents/Hardware/Flashing).
2. To verify that the Bridge Module is connected to the Master Module, simply initiate the  "TURN ON LED" command in the module's 'Settings' panel on the Developer Dashboard. If the LED turns on for about 5 seconds, the module is listening for commands from the cloud and  will also be able to send data.
3. Go to Device activity and paste the following command:
` {"path":"", "command":"down_ch_payload","value":[1,2,3]} `
Where [1,2,3] is the raw data sent to the bridge module (the payload).

----------

We hope that you'd utilize our Grove Bridge in your future projects. The realm of possibilities is as vast as your imagination. Arduino is just the beginning. Stay tuned for more Grove delights to come!


### License:

The MIT License (MIT)

Copyright (c) 2015 iThings4u GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
