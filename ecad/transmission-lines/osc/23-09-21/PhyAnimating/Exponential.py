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

		self.setWindowTitle("Exponential")

		self.slm00 = self.uiSliderAdd(1.0, 0.0, True, 'm00\t', '')
		self.slm01 = self.uiSliderAdd(1.0, -1.0, True, 'm01\t', '')
		self.slm10 = self.uiSliderAdd(1.0, 1.0, True, 'm10\t', '')
		self.slm11 = self.uiSliderAdd(1.0, 0.0, True, 'm11\t', '')

		self.uiLineAdd()

		self.slx0 = self.uiSliderAdd(1.0, 0.0, True, 'x0\t', '')
		self.slx1 = self.uiSliderAdd(1.0, 0.0, True, 'x1\t', '')

		self.uiLineAdd()

		self.txtMat = QLabel('')
		self.opts.addWidget(self.txtMat)

		self.ax = self.fig.add_subplot(1, 1, 1)

	def process(self):

		m00 = self.slm00.rVal
		m01 = self.slm01.rVal
		m10 = self.slm10.rVal
		m11 = self.slm11.rVal

		limT = 1.2
		limTN = 1.2

		self.txtMat.setText(f'\n\t\t{m00:.2}\t{m01:.2}\n\t\t{m10:.2}\t{m11:.2}')

		m = np.array([[m00, m01], [m10, m11]])

		a = 1.0
		h = np.linspace(-a, a, 9)
		x, y = np.meshgrid(h, h)
		p = np.row_stack((x.ravel(), y.ravel()))

		v = m @ p

		# ----------------------

		self.ax.clear()

		self.ax.quiver(p[0, :], p[1, :], v[0, :], v[1, :], width = 0.004)

		self.ax.set_xlim(-limT, limT)
		self.ax.set_ylim(-limTN, limTN)

		self.ax.set_xlabel('Julia');
		self.ax.set_ylabel('Romeo');

		self.ax.grid()

		# ----------------------

		self.fig.canvas.draw()

if __name__ == "__main__":

	pApp = DiracAnimating()

	while pApp.success == Pending:
		pApp.treeTick();

