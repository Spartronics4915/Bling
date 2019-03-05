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
import serial.tools.list_ports as list_ports
from ast import literal_eval
from serial.serialutil import SerialException

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
    'climb': b'4',
    'bling': b'8'
}
curSignal = b'0'
phase = ''

if len(sys.argv) < 2:
    print("No IP address supplied. Using default value ", end="")
    with open('config.txt') as file:
        ip = literal_eval(file.read())['default_ip']
        file.close()
        print(ip)
else:
    ip = sys.argv[1]


NetworkTables.initialize(server=ip)


def serialize():
    waited = False
    serialed = False
    time_wait = 0
    while not serialed:
        try:
            ser.baudrate = 9600
            with open('config.txt') as file:
                port = literal_eval(file.read())['serial_port']
                if port == "DEFAULT":
                    try:
                        print("Detecting")
                        ser.port = list(list_ports.comports()[0])[0]
                    except IndexError:
                        print("No serial port connected")
                        raise SerialException
                else:
                    ser.port = port
            ser.open()
            print("\n" + str(ser.port) if waited else ser.port)
            time.sleep(3)
            ser.write('0'.encode('ascii'))
            serialed = True
        except serial.serialutil.SerialException:
            if not waited:
                print("No serial detected. (0s)", end="")
                waited = True
            for x in range(0, 10):
                print("\rNo serial detected. (", time_wait, "s)", sep="", end="")
                time.sleep(1)
                time_wait += 1
                if sys.stdout is not None:
                    sys.stdout.flush()
    print("Detected")


serialize()


def valueChanged(table, key, value, isNew):
    if key == "GamePhase":
        global phase
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        print(phase, end=' ')
        phase = value
        print(phase)
        if value == "DISABLED":
            curSignal = commands['off']
            ser.write(curSignal)
        elif value == "AUTONOMOUS":
            curSignal = commands['auto']
            ser.write(curSignal)
        elif value == "TELEOP" and superstructureState == "CLIMB":
            curSignal = commands['climb']
            ser.write(curSignal)
        elif value == "TELEOP" and not reversed:
            curSignal = commands['teleopF']
            ser.write(curSignal)
        elif value == "TELEOP" and reversed:
            curSignal = commands['teleopR']
            ser.write(curSignal)
        elif value == "TEST":
            curSignal = commands['testing']
            ser.write(curSignal)


def superListener(table, key, value, isNew):
    if key == "Reverse":
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        reversed = value
        global phase
        print("Reversed:", reversed)
        if phase == "TELEOP" and not reversed:
            curSignal = commands['teleopF']
            ser.write(curSignal)
        elif phase == "TELEOP" and reversed:
            curSignal = commands['teleopR']
            ser.write(curSignal)
    elif key == "WantedState":
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        superstructureState = value


def blingListener(table, key, value, isNew):
    if key == "State" and value == "Acquired":
        print(f"valueChanged: key: '{key}'; value: {value}; isNew: {isNew}")
        ser.write(commands['bling'])
        time.sleep(3)
        ser.write(curSignal)


def connectionListener(connected, info):
    print(info, "; Connected=%s" % connected)


NetworkTables.addConnectionListener(connectionListener, immediateNotify=True)

robot = NetworkTables.getTable("SmartDashboard/Robot")
robot.addEntryListener(valueChanged)

superstructure = NetworkTables.getTable("SmartDashboard/Superstructure")
superstructure.addEntryListener(superListener)

vision = NetworkTables.getTable("SmartDashboard/Vision")
vision.addEntryListener(blingListener)

while True:
    try:
        print(str(ser.read()), sep='', end='')
    except SerialException:
        ser.close()
        serialize()
    time.sleep(1)
