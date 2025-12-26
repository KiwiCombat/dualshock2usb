# dualshock2usb

dualshock2usb is a project that will enable the use of DualShock 2 controllers on windows via an Arduino and XInput.

### Features

* Analogue triggers. (the DualShock 2 has analog buttons)

~~-Wireless controller ?~~

* Full big/small motor rumble support thanks to the ArduinoXInput library.

* Possibly a custom adapter, which will be much sleeker and compact than a bare arduino and jumper wires.

* Based on the XInput API, ensuring compatability with many systems and games.

### Wiring

<p>You will need 
one 10kohm resistor on the 5v pin (the ps2 controller runs at ~3.3v)
</p>

Controller	 Arduino<br>
Pin 1(Data)->Pin 10<br>
Pin 2(Command)->Pin 12<br>
Pin 3(motor power)->7.5-9v<br>
Pin 4(ground)-> ground pin<br>
Pin 5(Power)->3.3v pin or 5v pin with 10k resistor<br>
Pin 6(attention)->Pin 9<br>
Pin 7(clock)->Pin 11<br>

## Dependencies

This project uses the [Arduino-PS2X](https://github.com/madsci1016/Arduino-PS2X) and [ArduinoXInput](https://github.com/dmadison/ArduinoXInput) Libraries.
