import serial
import time
ser = serial.Serial('COM4', 38400)
time.sleep(2)

try:
	while True:
		b = ser.readline()

		try:
			s = b.decode().rstrip()
		except UnicodeDecodeError:
			continue
		except KeyboardInterrupt:
			break
		print(s)
finally:
	ser.close()