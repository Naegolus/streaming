#!/usr/bin/python

import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv(os.getcwd() + '/RigolDS0.csv')

#print(f"df = {df}")

t = np.array(df["Time(s)"])
ch1 = np.array(df["CH1(V)"]) * 0.5
ch2 = np.array(df["CH2(V)"])
ch2 = ch2 - ch2[0]
#ch3 = np.array(df["CH3(V)"])

#print(f"t = {t}")
#print(f"ch1 = {ch1}")

plt.plot(t, ch1, label = "Channel 1", color = 'y')
plt.plot(t, ch2, label = "Channel 2", color = 'c')
#plt.plot(t, ch3, label = "Channel 3")

plt.xlabel('Time')
plt.ylabel('Voltage')

plt.title('Stromi')
plt.legend()
plt.show()

