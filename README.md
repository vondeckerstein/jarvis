Jarvis
======

Expansible Arduino based home automation

Inspired by Tony Stark's J.A.R.V.I.S. in the Iron Man series, this project is intended to facilitate home automation using a small, dedicated computer(here a Raspberry Pi), and an expansible number of hardware IO interface devices based on the arduino platform.

For user input, Jarvis is currently designed to be accessed primarily through a web interface. In the medium term, multiple USB touchscreen capability is planned and IR remote control is a possibility. In the long term, voice recognition is planned(possibly in combination with the Jasper project).

For output, Jarvis is designed to interface any number of devices over USB, and to be extremely flexible in terms of IO supported. Currently functioning devices include relays, linear actuators, RGB strip LEDs, voltage sensing, current sensing, and temperature sensing. Many additional devices are planned and development is ongoing.

Hardware:
=========
![Jarvis Hardware](http://i.imgur.com/6hvVemG.png)

Currently, Jarvis's firmware is custom built for my hardware, pictured above and growing frequently. If you'd like to run Jarvis on your own equipment, I want to help. Feel free to email me and I would be happy to either develop or help you develop firmware to run your devices.


TO DO LIST/WISH LIST:
=====================
Arduino firmware:

*	debug Dware6 and 6b (6 for relayduino, 6b for UNO)
*	update commands documentation
*	add Temperature Sensor Functionality
*	add readAll command(for speed, instead of going back and forth over serial for each command)
*	(semi-)finalize firmware versions for universal hardware configurations for development purposes
*	long term vision: generate arduino firmware automatically for hardware described in configuration files in python, compile and upload using INOTOOL

Jarvis Python Host:

*	(Config Loader) Rewrite devices, link save states to be similar to rules(with attributes saved by name)
*	(Config Loader) Create Dummy Link class that emulates Arduino behavior for web interface testing
*	(New 'Rule" item) Add 'Thermostat' type rule, perform XXX if XXX
* 	(Relay class) Add optional Timeout(i.e. relay stays on XXX seconds/minutes/hours max)
*	(All channels) Come up with better common command specifier to tell UI what commands are available.

Web interface:

*	Replace CheckBoxes with decent looking Switches
*	Add visual gauges for analog voltage with arbitrary units
*	Add visual thermometers for temperature sensors
*	Improve layout
*	Add Alarm Widget that includes changing time
