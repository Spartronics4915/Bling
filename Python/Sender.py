import serial, time
import serial.tools.list_ports

print(list(serial.tools.list_ports.comports()[0]))

ser = serial.Serial()
print(ser.name)
ser.baudrate = 9600
ser.port = 'COM3'
ser.open()
time.sleep(3)
ser.write('0'.encode('ascii'))
try:
    while True:
        text = input("Command: ")
        ser.write(text.encode('ascii'))
except:
    ser.close()
    print("Goodbye")
