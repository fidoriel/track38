# track38

track38 is a cross-platform LEGO model train control system. It does support the LEGO PoweredUp system via Bluetooth, LEGO PowerFunctions via an Arduino and IR. Support for 9V RC LEGO trains is planned. It can control track switches equipped with a 4DBrixx Servo track switch brick as well.

Supported Platforms (Blutooth 4.0 or higher is needed):
 - Windows 8 and newer Intel 64bit
 - macOS Intel and ARM 64bit
 - Linux Intel 64bit and Raspberry Pi OS former raspbian 32bit

The program including the Arduino Client is written in C and C++.
The GUI depends on wxWidgets, the Bluetooth functionality is implemented with SimpleBLE and the PowerFunction capabilities are based on the work by Jurriaan Pruis.
The track38 application for Desktop is licensed under the GNU GPLv3. The Arduino Client is licensed under the MIT License.

