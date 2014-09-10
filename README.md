Introduction
------------
Welcome to the **Bridge library for Arduino**.
This repository contains a library and example sketch which allows you to connect the Wunderbar Bridge/Grove module to the Arduino UART and in just 10 minutes have an easy gateway to the **relayr open sensor cloud!**

## Getting Started 
### Import the Library

There are two methods to install the library:

#### Method 1: Import *zip* file
* Download the repository as **.zip** file by clicking in *"Download ZIP"* to the right of the screen.
* In the *Arduino IDE*, navigate to *"Sketch > Import Library"*. At the top of the drop down list, select the option to *"Add Library''*.
* You will be prompted to select the library you would like to add. Navigate to the *.zip* file's location and open it.
* Restart the *Arduino IDE* and go to the *"Sketch > Import Library"* menu. You should now see the library at the bottom of the drop-down menu. It is ready to be used in your sketch. The *Wunderbar Bridge* example should also be available now in the list.

#### Method 2: Using *git clone*
* First quit the *Arduino IDE*. 
* If you have **git** installed just go to the Arduino folder and do a `git clone https://github.com/relayr/Arduino-Bridge-Library.git`. 

    Under *Windows*, it will likely be under *"My Documents\Arduino\"*. For *Mac* users, it will likely be called *"Documents/Arduino/"*. On *Linux*, it will be a folder in your sketchbook. 
* Restart the *Arduino IDE* and go to the *"Sketch > Import Library"* menu. You should now see the library at the bottom of the drop-down menu. It is ready to be used in your sketch. The *Wunderbar Bridge* example should also be available now in the list.

The installed files should look like (in *Mac*):
*Documents/Arduino/libraries/WunderbarBridge/WunderbarBridge.cpp*
*Documents/Arduino/libraries/WunderbarBridge/WunderbarBridge.h*
*Documents/Arduino/examples/GroveBridge example/GroveBridge_example.ino*

### Flash the UART firmware in the Bridge Module:
In order to allow the *Bridge Module* to communicate over UART with the *Arduino*, is necessary to flash the Bridge UART Firmware (["sensor\_bridge\_fw\_UART.hex"](https://github.com/relayr/Arduino-Bridge-Library/blob/master/libraries/WunderbarBridge/sensor_bridge_fw_UART.hex)). This binary file is located inside the */libraries* folder.

Please refer to the [relayr documentation] (https://developer.relayr.io/documents/Welcome/Introduction) to learn how to flash a new *firmware* in a *Wunderbar Module*.