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

		self.setWindowTitle("Polyphase")

		self.t = np.linspace(0, 2*2*np.pi, 15)
		self.s = np.sin(self.t)

		self.t1 = self.t[0::3]
		self.p1 = self.s[0::3]

		self.t2 = self.t[1::3]
		self.p2 = self.s[1::3]

		self.t3 = self.t[2::3]
		self.p3 = self.s[2::3]

		self.axFull = self.fig.add_subplot(4, 1, 1)
		self.axPp1 = self.fig.add_subplot(4, 1, 2)
		self.axPp2 = self.fig.add_subplot(4, 1, 3)
		self.axPp3 = self.fig.add_subplot(4, 1, 4)

	def process(self):

		# ----------------------

		self.axFull.clear()

		self.axFull.stem(self.t, self.s, label = "Full")
		self.axPp1.stem(self.t1, self.p1, label = "Full")
		self.axPp2.stem(self.t2, self.p2, label = "Full")
		self.axPp3.stem(self.t3, self.p3, label = "Full")

		tmax = self.t[-1]
		self.axFull.set_xlim(0, tmax)
		self.axPp1.set_xlim(0, tmax)
		self.axPp2.set_xlim(0, tmax)
		self.axPp3.set_xlim(0, tmax)

		#self.axTime.set_ylim(-limTN, limTN)

		self.axFull.set_xlabel('n');
		self.axFull.set_ylabel('Value');

		#self.axFull.grid()

		# ----------------------

		self.fig.canvas.draw()

if __name__ == "__main__":

	pApp = DiracAnimating()

	while pApp.success == Pending:
		pApp.treeTick();

