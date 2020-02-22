# Conversions found here: https://www.electronicwings.com/sensors-modules/mpu6050-gyroscope-accelerometer-temperature-sensor-module

import numpy as np
import time
import serial
from pprint import pprint
from matplotlib import pyplot as plt
from sys import exit

def getAccel(serIn):
	b = ser.readline()
	data = b.decode().rstrip()
	accel = np.array([int(x) for x in data.split(',')])
	return accel

# Set up serial input
try:
	ser = serial.Serial('COM4', 38400, timeout=.1)
except serial.SerialException as e:
	print('ERROR: Serial port could not be opened:')
	print(e)
	exit()

print('Serial connection open.')
time.sleep(2)

data = np.zeros(7)
read = np.zeros(7)

period = 5

try:
	tic = time.time()
	print('Recording...')
	while time.time() < tic + period:
		read[0] = time.time() - tic
		try:
			read[1:] = getAccel(ser)
		except UnicodeDecodeError:
			continue
		except ValueError:
			continue
		except KeyboardInterrupt:
			break
		data = np.vstack([data, read])
finally:
	ser.close()
	print('Done. Closing serial connection.')

# Get rid of zero row
data = data[1:, :].astype(float)

# Convert readings to g and deg/s
data[:, 1:4] = data[:, 1:4] * 1/16384
data[:, 4:] = data[:, 4:] * 1/131

print('Collected', np.shape(data)[0], 'datapoints in', period, 'seconds.')

# Plot data
facc = plt.figure(1)
plt.plot(data[:, 0], data[:, 1:4])
plt.xlabel('Time [s]')
plt.ylabel('Acceleration [g]')
plt.legend(['xddot', 'yddot', 'zddot'])
plt.grid(True)
facc.show()

fgyr = plt.figure(2)
plt.plot(data[:, 0], data[:, 4:])
plt.xlabel('Time [s]')
plt.ylabel('Angular velocity [deg/s]')
plt.legend(['omega_x', 'omega_y', 'omega_z'])
plt.grid(True)
fgyr.show()

input()