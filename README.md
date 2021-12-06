# track38

track38 is a cross-platform LEGO model train control system. It does support the LEGO PoweredUp system via Bluetooth, LEGO PowerFunctions via an Arduino and IR. Support for 9V RC LEGO trains is planned. It can control track switches equipped with a 4DBrixx Servo track switch brick as well.

Supported Platforms (Blutooth 4.0 or higher is needed):
 - Windows 8 and newer Intel 64bit
 - macOS Intel and ARM 64bit
 - Linux Intel 64bit and Raspberry Pi OS former raspbian 32bit

The program including the Arduino Client is written in C and C++.
The GUI depends on wxWidgets, the Bluetooth functionality is implemented with SimpleBLE and the PowerFunction capabilities are based on the work by Jurriaan Pruis.
The track38 application for Desktop is licensed under the GNU GPLv3. The Arduino Client is licensed under the MIT License.

# How it Works

macOS note: Please allow Bluetooth Permission if track38 asks for. Otherwise it will terminate and you have to go to the Settings->Security->Privacy and tick track38. If track38 does not have permissions it will terminate.

PowerFunctions and track switches requirements.
    Upload the track38 Arduino Firmware to your board. This needs to be done only once.
    This can be a little bit tricky: Download and install the Arduino IDE on your machine. Plug in the Arduino. Open the Sketch and select port and device from the menue. Hit upload ans wait a few seconds.

Add Trains:
Go to "Edit Trains" and click "New" on the left choose the controller and fill in the information. 

Case Bluetooth:
    Scan for Hubs, press the Button on the PoweredUp controller and select your train's controller via name or MacAdress on the dialog window. Please make sure your Hubs have different names.
    You can add as many trains as you like. But connections at the same time are limited to 7. (numer can change depending on your system)
Case PowerFunctions:
    Refresh the ComPorts and choose your Arduino (it has to be plugged in).
    Conect an 940nm (and a resistor) to a GPIO pin and enter the pin in the train config for every PowerFunctions train you want to control with this LED.
    Please Note: if you have a large layout with hidden tracks you might want to add more IR LEDs to reach all corners. For IR signals can not be verified if the train has recieved it.
    The number limit of Lego PowerFunctions trains is 8. Two per chanel * four channels.
    You can add as many Arduino controller as you like. But connection is limited by your computers power deliver abilities. You might use external power supplied or powered hubs.

Add Trackswitches:
    Go to "Edit map". Drag symbols from the Symbol Picker to recreate yout train layout on screen.
    Connect the servo with your Arduino.
    Arduino 5V to servo red
    Arduino GND to servo black/brown
    Now choose an empty GPIO pin. Connect this pin to servo yellow.
    Right Click on the map -> DragMode
    Click on the track switch you want to control. Refresh the comport and select your Arduino.
    Enter the pin you connected the servo to.
    Put the track switch servo thing on to your Lego Trackswitch.

Now you are good to go:
    Go to Control
    Your Arduino Controller should connect automaticly.
    Your trains are shown on the "Train Control" Panel. If the stop button is greyed out the connection was not succesful. Plug in the Arduino and check if you always selected an port for trains or switches which need one. If so go to Control->Reload Connections menue option or hit Ctrl-R to reload.

    You have to pair your PoweredUp hubs each manually. Press the Bluetooth Symbol next to the train silder. Now track38 will search for 5 seconds for the Hub. Please press the button on the hub within this time frame. If the connection was succesful the Bluetooth sing should show a small disconnection icon.

    By pressing on the switches in Control you can toggle them and it will show the current switch position on the map. If track38 does not know the position the first toggle will switch them straight if necessary.

    By sliding the speed-slider to the left or right, the train will start move. With the stop sing the train will stop. Terminating track38 will stop and disconnect all trains.

# TODO 
 - Make .ini file loading save
 - add 9V RC train
 - improove Serial conection method
 - add App wide settings
 - change all serial ports once
 - add PoweredUp track switches
 - add import/export
 - add reset App option
 - add update notifications
 - add more user warnings & exeptions
