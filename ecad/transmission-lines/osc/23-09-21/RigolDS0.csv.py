#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np
import csv

with open('RigolDS0.csv', 'r') as f:
	reader = csv.reader(f)
	data = list(reader)

t = data[0, :]
v = data[1, :]

print(f"t = {t}")

exit(1)

plt.plot(t, v, label = "Sig")

plt.xlabel('Time')
plt.ylabel('Voltage')

plt.title('Stromi')
plt.legend()
plt.show()

