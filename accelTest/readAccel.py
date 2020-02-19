from matplotlib import pyplot as plt
import matplotlib.animation as animation
import datetime as dt
from itertools import count
from time import time
import serial

''' Set up serial input '''
arduino = serial.Serial('COM4', 9600, timeout=.1)

''' Set up live plot '''
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

def animate(i, xs, ys):
	accel = [0, 0, 0]
	# Read data from serial
	data = arduino.readline()[:-2]
	if data:
		data = data.split(b',')
		accel = [int(x) for x in data]

	xddot = accel[0]
	yddot = accel[1]
	zddot = accel[2]

	xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
	ys.append(xddot)

	xs = xs[-20:]
	ys = ys[-20:]

	ax.clear()
	ax.plot(xs, ys)

	plt.xticks(rotation=45, ha='right')
	plt.ylim([-4000, 4000])
	plt.subplots_adjust(bottom=0.30)
	plt.title('Acceleration')
	plt.ylabel('xddot')

ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=100)
plt.show()