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
import time
from networktables import NetworkTables

import serial, time
import serial.tools.list_ports
from ast import literal_eval

print(list(serial.tools.list_ports.comports()[0]))

ser = serial.Serial()
ser.baudrate = 9600
with open('config.txt') as file:
        ser.port = literal_eval(file.read())['arduino_port']
        file.close()
        print(ser.port)
ser.open()
time.sleep(3)
ser.write('0'.encode('ascii'))

commands = {
    "off": '0'.encode('ascii'),
    'teleopF': '1'.encode('ascii'),
    'teleopR' : '2'.encode('ascii'),
    'auto': '3'.encode('ascii'),
    'testingF': '4'.encode('ascii'),
    'testingR': '5'.encode('ascii')
}

# To see messages from networktables, you must setup logging
import logging

logging.basicConfig(level=logging.DEBUG)

ip = ""

if len(sys.argv) != 2:
    print("No IP address supplied. Using default value ", end="")
    with open('config.txt') as file:
        ip = literal_eval(file.read())['default_ip']
        file.close()
        print(ip)
else:
    ip = sys.argv[1]

NetworkTables.initialize(server=ip)

def valueChanged(table, key, value, isNew):
    if key == "GamePhase":
        print("valueChanged: key: '%s'; value: %s; isNew: %s" % (key, value, isNew))
        if value == "DISABLED":
            ser.write(commands['off'])
        elif value == "AUTONOMOUS":
            ser.write(commands['auto'])
        elif value == "TELEOP":
            ser.write(commands['teleopF'])
        elif value == "TEST":
            ser.write(commands['testingF'])


def connectionListener(connected, info):
    print(info, "; Connected=%s" % connected)


NetworkTables.addConnectionListener(connectionListener, immediateNotify=True)

sd = NetworkTables.getTable("SmartDashboard/Robot")
sd.addEntryListener(valueChanged)

while True:
    time.sleep(1)
