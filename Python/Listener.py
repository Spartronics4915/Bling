#!/usr/bin/env python3
#
# This is a NetworkTables client (eg, the DriverStation/coprocessor side).
# You need to tell it the IP address of the NetworkTables server (the
# robot or simulator).
#
# This shows how to use a listener to listen for changes in NetworkTables
# values. This will print out any changes detected on the SmartDashboard
# table.
#

import os
import sys
from networktables import NetworkTables
import logging
import serial
import time
import serial.tools.list_ports
from ast import literal_eval

logging.basicConfig(level=logging.DEBUG)

ip = "10.49.15.2"
reversed = False
superstructureState = "YEEEEET"
ser = serial.Serial()
commands = {
    'init': b'7',
    'off': b'0',
    'teleopF': b'1',
    'teleopR': b'2',
    'auto': b'3',
    'testing': b'6',
    'climb': b'4'
}

if len(sys.argv) < 2:
    print("No IP address supplied. Using default value ", end="")
    with open('config.txt') as file:
        ip = literal_eval(file.read())['default_ip']
        file.close()
        print(ip)
else:
    ip = sys.argv[1]

def serialize():
    waited = False
    serialed = False
    while not serialed:
        try:
            ser.baudrate = 9600
            with open('config.txt') as file:
                port = literal_eval(file.read())['serial_port']
                ser.port = port if port != "DEFAULT" else list(serial.tools.list_ports.comports()[0])[0]
            ser.open()
            print("\n" + str(ser.port) if waited else ser.port)
            time.sleep(3)
            ser.write('0'.encode('ascii'))
            serialed = True
        except serial.serialutil.SerialException:
            if not waited:
                print("No serial detected.", end="")
                waited = True
            for x in range(0, 30):
                print(".", end="")
                time.sleep(1/3)
                if sys.stdout is not None:
                    sys.stdout.flush()

serialize()

NetworkTables.initialize(server=ip)


def superListener(table, key, value, isNew):
    if key == "Reverse":
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        reversed = value
    elif key == "WantedState":
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        superstructureState = value


def valueChanged(table, key, value, isNew):
    if key == "GamePhase":
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        if value == "DISABLED":
            ser.write(commands['off'])
        elif value == "AUTONOMOUS":
            ser.write(commands['auto'])
        elif value == "TELEOP" and superstructureState == "CLIMB":
            ser.write(commands['climb'])
        elif value == "TELEOP" and not reversed:
            ser.write(commands['teleopF'])
        elif value == "TELEOP" and reversed:
            ser.write(commands['teleopR'])
        elif value == "TEST":
            ser.write(commands['testing'])


def connectionListener(connected, info):
    print(info, "; Connected=%s" % connected)


NetworkTables.addConnectionListener(connectionListener, immediateNotify=True)

robot = NetworkTables.getTable("SmartDashboard/Robot")
robot.addEntryListener(valueChanged)

superstructure = NetworkTables.getTable("SmartDashboard/Superstructure")
superstructure.addEntryListener(superListener)

while True:
    try:
        print(str(ser.read()), sep='', end='')
    except:
        ser.close()
        serialize()
    time.sleep(1)
