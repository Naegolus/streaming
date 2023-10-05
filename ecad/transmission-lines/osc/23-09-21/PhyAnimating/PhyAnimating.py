#!/usr/bin/env python

import matplotlib as mpl
import matplotlib.figure
import matplotlib.backends.backend_qt5agg

import PyQt5
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

import math
import numpy as np
import scipy.integrate

import sys

Pending = 0
Positive = 1

class QHLine(QFrame):
	def __init__(self):
		super(QHLine, self).__init__()
		self.setFrameShape(QFrame.HLine)
		self.setFrameShadow(QFrame.Sunken)

class PhyAnimating(PyQt5.QtWidgets.QMainWindow):

	def __init__(self):

		self.app = PyQt5.QtWidgets.QApplication([])

		super().__init__()

		self.uiBaseCreate()

		self.initExecuted = False
		self.success = Pending

	def uiBaseCreate(self):

		self.width = 1280
		self.height = 800
		self.slLbls = []

		self.resize(self.width, self.height)
		self.setMinimumSize(400, 300)

		self.opts = QVBoxLayout()

		layOuter = QGridLayout()
		layOuter.addLayout(self.opts, 0, 0, 1, 1)

		self.fig = mpl.figure.Figure()
		mpl.backends.backend_qt5agg.FigureCanvasQTAgg(self.fig)
		self.fig.set_tight_layout(True)

		layOuter.addWidget(self.fig.canvas, 0, 1, 1, 1)
		layOuter.setColumnStretch(0, 1)
		layOuter.setColumnStretch(1, 3)

		centWid = QWidget(self)
		self.setCentralWidget(centWid)
		centWid.setLayout(layOuter)

	def uiSliderAdd(self, rMax, rStart, twoSided, prefix, unit):

		sl = QSlider(Qt.Horizontal)

		if twoSided:
			sl.setRange(-100, 100)
		else:
			sl.setRange(0, 100)

		sl.setSingleStep(25)
		v = 100 * rStart / rMax
		sl.setValue(int(v))
		self.opts.addWidget(sl)

		sl.rVal = rStart
		sl.rMax = rMax

		lblStr = ''
		if prefix:
			lblStr += f'{prefix}{rStart:.2}'
			if unit:
				lblStr += f' [{unit}]'

		lbl = QLabel(lblStr)
		self.opts.addWidget(lbl)

		lbl.prefix = prefix
		lbl.unit = unit
		lbl.sl = sl
		lbl.rMax = rMax
		self.slLbls.append(lbl)

		sl.valueChanged.connect(self.uiLabelsUpdate)
		sl.valueChanged.connect(self.process)

		return sl

	def uiLineAdd(self):
		self.opts.addWidget(QHLine())

	def uiLabelsUpdate(self):

		for l in self.slLbls:
			v = l.rMax * l.sl.value() / 100

			l.sl.rVal = v

			lblStr = ''
			if l.prefix:
				lblStr += f'{l.prefix}{v:.2}'
				if l.unit:
					lblStr += f' [{l.unit}]'

			l.setText(lblStr)

	def __del__(self):
		pass

	def treeTick(self):

		self.initialize()
		self.opts.addStretch(1)

		self.process()

		super().show()
		self.app.exec_()

		self.success = Positive

