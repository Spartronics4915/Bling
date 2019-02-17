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

import serial, time
import serial.tools.list_ports
from ast import literal_eval

##print(list(serial.tools.list_ports.comports()[0]))

waited = False


while True:
    try:
        ser = serial.Serial()
        ser.baudrate = 9600
        with open('config.txt') as file:
                ser.port = literal_eval(file.read())['serial_port']
                file.close()
        ser.open()
        print("\n" + str(ser.port) if waited else ser.port)
        time.sleep(3)
        ser.write('0'.encode('ascii'))
        break
    except:
        if not waited:
            print("No serial detected.", end="")
            waited = True
        for x in range(0,30):
            print(".", end="")
            time.sleep(1/3)
            if sys.stdout is not None:
                sys.stdout.flush()

        


commands = {
    "init": '7'.encode('ascii'),
    "off": '0'.encode('ascii'),
    'teleopF': '1'.encode('ascii'),
    'teleopR' : '2'.encode('ascii'),
    'auto': '3'.encode('ascii'),
    'testing': '6'.encode('ascii'),
    'climb': '4'.encode('ascii')
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

reversed = False

superstructureState = "YEEEEET"

def superListener(table, key, value, isNew):
    if key == "Reverse":
        print("valueChanged: key: '%s'; value: %s; isNew: %s" % (key, value, isNew))
        reversed = value
    if key == "WantedState":
        print("valueChanged: key: '%s'; value: %s; isNew: %s" % (key, value, isNew))
        superstructureState = value;
        

def valueChanged(table, key, value, isNew):
    if key == "GamePhase":
        print("valueChanged: key: '%s'; value: %s; isNew: %s" % (key, value, isNew))
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

while True:
    try:
        NetworkTables.addConnectionListener(connectionListener, immediateNotify=True)

        robot = NetworkTables.getTable("SmartDashboard/Robot")
        robot.addEntryListener(valueChanged)

        superstructure = NetworkTables.getTable("Smartdashboard/Superstructure")
        superstrucutre.addEntryListener(superListener)
        break
    except:
        print("Not connected in.")
        time.sleep(30)
while True:
    time.sleep(1)
