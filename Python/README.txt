Sender.py
	Can send data over the serial port.
	Usage: 
		python3 ./Sender.py

Listener.py:
	Link the network tables to the arduino.
	Usage:
		python3 ./Listener.py #Opens default ip
		python3 ./Listener.py <ip> #opens ip port
Config.txt:
	'default_ip' the default ip address if none is specified.
	'serial_port' the port the arduino uses for serial communication. e.g. COM3 for Windows, /dev/ttyACM0 for linux.
Readme.txt:
	Provides instructions for other files.

		