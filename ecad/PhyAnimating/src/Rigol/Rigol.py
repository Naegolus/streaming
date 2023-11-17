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

		df = pd.read_csv(os.getcwd() + '/RigolDS0.csv')

		self.t = np.array(df["Time(s)"])
		self.ch1 = np.array(df["CH1(V)"]) * 0.5
		self.ch2 = np.array(df["CH2(V)"])
		self.ch2 = self.ch2 - self.ch2[0]

		self.axTime  = self.fig.add_subplot(3, 1, 1)
		self.axFreq1 = self.fig.add_subplot(3, 1, 2)
		self.axFreq1twin = self.axFreq1.twiny()
		self.axAttenuation = self.fig.add_subplot(3, 1, 3)

		z = 30
		n = 15

		pads = np.zeros([1, n * self.ch1.size])
		self.ch1p = np.append(self.ch1, pads)

		self.f1 = np.abs(np.fft.fft(self.ch1))
		self.f1z = self.f1[0 : z]

		self.f1p = np.abs(np.fft.fft(self.ch1p))
		self.f1pz = self.f1p[0 : (1 + n) * z]

		self.ch2p = np.append(self.ch2, pads)

		self.f2 = np.abs(np.fft.fft(self.ch2))
		self.f2z = self.f2[0 : z]

		self.f2p = np.abs(np.fft.fft(self.ch2p))
		self.f2pz = self.f2p[0 : (1 + n) * z]
		self.f2pzf = np.linspace(0, self.f2pz.size - 1, self.f2pz.size) / self.ch2p.size * 1000

		self.fMax = np.amax(self.f2pzf)

		self.a = 20 * np.log10(self.f1pz / self.f2pz)

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

		limA = 500

		self.axFreq1.clear()

		self.axFreq1.plot(self.f2pzf, self.f1pz, label = "Freq Ch. 1", color = 'y')
		self.axFreq1.plot(self.f2pzf, self.f2pz, label = "Freq Ch. 2", color = 'c')
		self.axFreq1.set_xlim(0, self.fMax)

		#self.axFreq1.set_xlabel('Normalized Frequency @ 2 GSa/s');
		self.axFreq1.set_xlabel('Frequency [MHz]');
		self.axFreq1.set_ylabel('Amplitude [?]');

		self.axFreq1.set_ylim(-limA/10, limA)

		self.axFreq1twin.stem(self.f1z, label = "Freq Ch. 1")
		self.axFreq1twin.set_xlim(0, self.f2z.size)
		self.axFreq1twin.get_xaxis().set_visible(False)

		# ----------------------

		self.axAttenuation.plot(self.f2pzf, self.a, label = "Attenuation", color = 'b')
		self.axAttenuation.set_xlim(0, self.fMax)

		self.axAttenuation.grid()

		self.axAttenuation.set_xlabel('Frequency [MHz]');
		self.axAttenuation.set_ylabel('Attenuation [dB]');

		# ----------------------

		self.fig.canvas.draw()

if __name__ == "__main__":

	pApp = DiracAnimating()

	while pApp.success == Pending:
		pApp.treeTick();

