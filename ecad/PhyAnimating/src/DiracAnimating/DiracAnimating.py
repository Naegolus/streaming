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

from PhyAnimating import *

class DiracAnimating(PhyAnimating):

	def initialize(self):

		self.setWindowTitle("Dirac")

		#btn1 = QPushButton("Test")
		#self.opts.addWidget(btn1)

		self.slTime = self.uiSliderAdd(1, 0.0, True, 'Time offset\t', 's')
		self.slFreq = self.uiSliderAdd(5, 0.0, True, 'Frequency\t', 'Hz')
		self.slPhi = self.uiSliderAdd(2 * math.pi, 0.0, False, 'Phi\t\t', 'Rad')

		self.axTime = self.fig.add_subplot(1, 2, 1, projection = '3d')
		self.axFreq = self.fig.add_subplot(1, 2, 2, projection = '3d')

	def process(self):

		f0 = self.slFreq.rVal
		t0 = self.slTime.rVal
		phi0 = self.slPhi.rVal

		# ----------------------

		t = np.linspace(-1, 1, 400)
		s = np.exp(1j * (t * 2 * math.pi * f0 + phi0))
		y = np.imag(s)
		z = np.real(s)
		a = np.zeros(t.size)

		self.axTime.clear()
		self.axTime.plot(t, y, z, 'blue')
		self.axTime.plot(t, a, a, 'grey', linestyle = 'dashed')

		self.axTime.plot([0, 0], [0, 0], [0, 3],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime.plot([0, 0], [0, 1.5], [0, 0],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime.plot([t0, t0], [0, 0], [0, 1],
						color = 'orange', alpha = 1.0, lw = 1)

		limT = 1.0
		limTN = 1.2

		self.axTime.set_xlim(-limT, limT)
		self.axTime.set_ylim(-limTN, limTN)
		self.axTime.set_zlim(-limTN, limTN)

		self.axTime.set_xlabel('Time [s]');
		self.axTime.set_ylabel('Im');
		self.axTime.set_zlabel('Re');

		self.axTime.grid()

		# ----------------------

		fMax = self.slFreq.rMax
		f = np.linspace(-fMax, fMax, 400)
		sw = np.exp(-1j * 2 * math.pi * f * t0)
		y = np.imag(sw)
		z = np.real(sw)
		a = np.zeros(f.size)

		self.axFreq.clear()
		self.axFreq.plot(f, y, z, 'orange')
		self.axFreq.plot(f, a, a, 'grey', linestyle = 'dashed')

		self.axFreq.plot([0, 0], [0, 0], [0, 3],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axFreq.plot([0, 0], [0, 1.5], [0, 0],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axFreq.plot([f0, f0], [0, np.sin(phi0)], [0, np.cos(phi0)],
						color = 'blue', alpha = 1.0, lw = 1)

		limF = fMax * limT

		self.axFreq.set_xlim(-limF, limF)
		self.axFreq.set_ylim(-limTN, limTN)
		self.axFreq.set_zlim(-limTN, limTN)

		self.axFreq.set_xlabel('Freq [Hz]');
		self.axFreq.set_ylabel('Im');
		self.axFreq.set_zlabel('Re');

		self.axFreq.grid()

		# ----------------------

		self.fig.canvas.draw()

if __name__ == "__main__":

	pApp = DiracAnimating()

	while pApp.success == Pending:
		pApp.treeTick();

