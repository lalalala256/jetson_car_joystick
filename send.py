import serial
import time
ser = serial.Serial("/dev/ttyTHS1", 115200)
ser.bytesize=8
ser.stopbits=1
ser.parity="N"
send_data = "\xff\xfe\x01\x00\x00\xf0\xf0\x00\x00\x70"
for i in range(0, 20):
    ser.write(b"\xff\xfe\x01\x00\x03\x00\x00\x00\x00\x77")
    read_msg = ser.read(8)
    print(i, read_msg)
    print(i)

time.sleep(5)
ser.write(b"\xff\xfe\x01\x00\x00\x00\x00\x00\x00\xff")
ser.close()
