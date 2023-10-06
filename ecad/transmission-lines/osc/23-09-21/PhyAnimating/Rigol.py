#!/usr/bin/python3

#  This file is part of the DSP-Crowd project
#  https://www.dsp-crowd.com
#
#  Author(s):
#      - Johannes Natter, office@dsp-crowd.com
#
#  File created on 05.10.2023
#
#  Copyright (C) 2023-now Authors and www.dsp-crowd.com
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program. If not, see <http://www.gnu.org/licenses/>.

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

