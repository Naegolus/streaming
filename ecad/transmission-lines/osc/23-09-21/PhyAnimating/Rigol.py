#!/usr/bin/python3

import os
import pandas as pd

from PhyAnimating import *

class DiracAnimating(PhyAnimating):

	def initialize(self):

		self.setWindowTitle("Stromi")

		df = pd.read_csv(os.getcwd() + '/../RigolDS0.csv')

		self.t = np.array(df["Time(s)"])
		self.ch1 = np.array(df["CH1(V)"]) * 0.5
		self.ch2 = np.array(df["CH2(V)"])
		self.ch2 = self.ch2 - self.ch2[0]

		self.axTime = self.fig.add_subplot(2, 1, 1)
		self.axFreq = self.fig.add_subplot(2, 1, 2)

	def process(self):

		# ----------------------

		self.axTime.clear()

		self.axTime.plot(self.t, self.ch1, label = "Channel 1", color = 'y')
		self.axTime.plot(self.t, self.ch2, label = "Channel 2", color = 'c')

		limT = 1.0
		limTN = 1.2

		#self.axTime.set_xlim(-limT, limT)
		#self.axTime.set_ylim(-limTN, limTN)

		self.axTime.set_xlabel('Time [s]');
		self.axTime.set_ylabel('Voltage [V]');

		self.axTime.grid()

		# ----------------------

		self.fig.canvas.draw()

if __name__ == "__main__":

	pApp = DiracAnimating()

	while pApp.success == Pending:
		pApp.treeTick();

