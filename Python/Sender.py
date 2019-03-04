import serial, time
import serial.tools.list_ports
from ast import literal_eval

print(list(serial.tools.list_ports.comports()[0]))

ser = serial.Serial()
print(ser.name)
ser.baudrate = 9600
with open('config.txt') as file:
    ser.port = literal_eval(file.read())['serial_port']
ser.open()
time.sleep(3)
ser.write('0'.encode('ascii'))
mode = input("mode")
try:
    while True:
        if mode == "send":
            text = input("Command: ")
            ser.write(text.encode('ascii'))
        elif mode == "read":
            print(ser.read(1).decode('ascii'), sep='', end='')
except:
    ser.close()
    print("Goodbye")
