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

# Global Variables.
ip = ""
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

# Event Listeners


def superListener(table, key, value, isNew):
    if key == "Reverse":
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        reversed = value
    if key == "WantedState":
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


# Parsing Command Line arguments

def parse_args():
    if len(sys.argv) < 2:
        print("No IP address supplied. Using default value ", end="")
        with open('config.txt') as file:
            ip = literal_eval(file.read())['default_ip']
            file.close()
            print(ip)
    else:
        ip = sys.argv[1]

# Defining Serial Port


def serial_init():
    # Print out the first connected port.
    waited = False
    serialed = False
    while not serialed:
        try:
            print("Test")
            ser.baudrate = 9600
            with open('config.txt') as file:
                ser.port = literal_eval(file.read())['serial_port']
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


def network():
    NetworkTables.initialize(server=ip)

    networked = False

    while not networked:
        try:
            NetworkTables.addConnectionListener(
                connectionListener,
                immediateNotify=True
                )

            robot = NetworkTables.getTable(
                "SmartDashboard/Robot"
                )
            robot.addEntryListener(valueChanged)

            superstructure = NetworkTables.getTable(
                "Smartdashboard/Superstructure"
                )
            superstrucutre.addEntryListener(superListener)
            networked = True
        except NameError:
            print("Not connected to robot.")
            time.sleep(30)


def main():
    parse_args()
    serial_init()
    # To see messages from networktables, you must setup logging
    logging.basicConfig(level=logging.DEBUG)
    network()
    while True:
        time.sleep(1)


main()
