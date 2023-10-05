#!/usr/bin/python3

from PhyAnimating import *

class DiracAnimating(PhyAnimating):

	def initialize(self):

		self.setWindowTitle("Convolution")

		self.flip = QCheckBox("Flip")
		self.flip.stateChanged.connect(self.process)
		self.opts.addWidget(self.flip)

		self.slTime = self.uiSliderAdd(4.5, 0.0, True, 'Time offset\t', 's')

		self.axTime1 = self.fig.add_subplot(2, 2, 1, projection = '3d')
		self.axTime2 = self.fig.add_subplot(2, 2, 3, projection = '3d')
		self.axTime3 = self.fig.add_subplot(1, 2, 2, projection = '3d')

	def rect(self, t):

		y = np.zeros(t.size)

		for i in range(len(t)):
			if t[i] >= 0 and t[i] < 2:
				y[i] = 1

		return y

	def triang(self, t):

		y = np.zeros(t.size)

		for i in range(len(t)):
			tc = t[i]

			if tc >= 0 and tc < 0.5:
				y[i] = 2 * tc

			if tc >= 0.5 and tc < 1:
				y[i] = - 2 * tc + 2

		return y

	def process(self):

		t0 = self.slTime.rVal

		limT = 5.4
		limTN = 1.2

		# ----------------------

		t = np.linspace(-5, 5, 400)
		dt = t[1] - t[0]

		if self.flip.checkState() == Qt.Checked:
			s1 = self.triang(t)
			s2 = self.rect(t0 - t)
			s3 = self.rect(t)
		else:
			s1 = self.rect(t)
			s2 = self.triang(t0 - t)
			s3 = self.triang(t)

		a = np.zeros(t.size)

		self.axTime1.clear()

		y = np.imag(s1)
		z = np.real(s1)
		self.axTime1.plot(t, y, z, 'blue')

		y = np.imag(s2)
		z = np.real(s2)
		self.axTime1.plot(t, y, z, 'orange')

		self.axTime1.plot(t, a, a, 'grey', linestyle = 'dashed')

		self.axTime1.plot([0, 0], [0, 0], [0, 3],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime1.plot([0, 0], [0, 1.5], [0, 0],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime1.set_xlim(-limT, limT)
		self.axTime1.set_ylim(-limTN, limTN)
		self.axTime1.set_zlim(-limTN, limTN)

		self.axTime1.set_xlabel('Time [s]');
		self.axTime1.set_ylabel('Im');
		self.axTime1.set_zlabel('Re');

		self.axTime1.grid()

		# ----------------------

		self.axTime2.clear()

		s = s1 * s2
		su = np.sum(s) * dt

		y = np.imag(s)
		z = np.real(s)
		self.axTime2.plot(t, y, z, 'red')

		self.axTime2.plot(t, a, a, 'grey', linestyle = 'dashed')

		self.axTime2.plot([0, 0], [0, 0], [0, 3],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime2.plot([0, 0], [0, 1.5], [0, 0],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime2.set_xlim(-limT, limT)
		self.axTime2.set_ylim(-limTN, limTN)
		self.axTime2.set_zlim(-limTN, limTN)

		self.axTime2.set_xlabel('Time [s]');
		self.axTime2.set_ylabel('Im');
		self.axTime2.set_zlabel('Re');

		self.axTime2.grid()

		# ----------------------

		self.axTime3.clear()

		s = np.convolve(s1, s3, 'same') * dt

		y = np.imag(s)
		z = np.real(s)
		self.axTime3.plot(t, y, z, 'green')

		self.axTime3.plot(t, a, a, 'grey', linestyle = 'dashed')

		self.axTime3.plot([0, 0], [0, 0], [0, 3],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime3.plot([0, 0], [0, 1.5], [0, 0],
						color = 'grey', linestyle = 'dashed', alpha = 1.0, lw = 1)

		self.axTime3.plot([t0, t0], [0, 0], [0, su],
						color = 'red', alpha = 1.0, lw = 1)

		self.axTime3.set_xlim(-limT, limT)
		self.axTime3.set_ylim(-limTN, limTN)
		self.axTime3.set_zlim(-limTN, limTN)

		self.axTime3.set_xlabel('Time [s]');
		self.axTime3.set_ylabel('Im');
		self.axTime3.set_zlabel('Re');

		self.axTime3.grid()

		# ----------------------

		self.fig.canvas.draw()

if __name__ == "__main__":

	pApp = DiracAnimating()

	while pApp.success == Pending:
		pApp.treeTick();

