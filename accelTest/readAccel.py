# Conversions found here: https://www.electronicwings.com/sensors-modules/mpu6050-gyroscope-accelerometer-temperature-sensor-module

import numpy as np
import time
import serial
from pprint import pprint
from matplotlib import pyplot as plt
from sys import exit

g = 9.81
# Gravitational accel in Waterloo, ON
gLocal = 9.8224
# 95% CI
z = 1.96

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

# Convert readings to m/s and deg/s
data[:, 1:4] = data[:, 1:4] * 1/16384 * g
data[:, 4:] = data[:, 4:] * 1/131

print('Collected', np.shape(data)[0], 'datapoints in', period, 'seconds.')

# Plot data
facc = plt.figure(1)
plt.plot(data[:, 0], data[:, 1:4])
plt.xlabel('Time [s]')
plt.ylabel('Acceleration [m/s^2]')
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

# Process data
Npoints = np.shape(data[:, 1:4])[0]
accelVecs = data[:, 1:4]
accelRMS = np.sqrt(np.sum(accelVecs**2, 0)/Npoints)
accelMean = np.mean(accelVecs, 0)
accelStd = np.linalg.norm(np.std(accelVecs, 0))
# print('RMS acceleration vector:', accelRMS, '[m/s^2]')
# print('Magnitude:', np.linalg.norm(accelRMS), '[m/s^2]')
print('Mean acceleration vector:', accelMean, '[m/s^2]')
print('Magnitude:', np.linalg.norm(accelMean), '+-', accelStd*z/np.sqrt(Npoints), '[m/s^2]')

input()