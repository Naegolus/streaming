#!/usr/bin/python

#  This file is part of the DSP-Crowd project
#  https://www.dsp-crowd.com
#
#  Author(s):
#      - Johannes Natter, office@dsp-crowd.com
#
#  File created on 07.07.2022
#
#  Copyright (C) 2022 Authors and www.dsp-crowd.com
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

import subprocess

from Processing import *
from libLed import *

def millis():
	return time_ns() // 10**6

CheckWaitTimeMs = 100

class Dm4Flashing(Processing):

	def initialize(self):

		self.procDbgLog("Initialized")

		self.state = self.SystemInit
		self.mStart = 0

		return Positive

	def process(self):

		self.state()

		return Pending

	def SystemInit(self):

		green()

		self.mStart = millis()
		self.state = self.BoardAttachedWait

	def BoardAttachedWait(self):

		if millis() - self.mStart < CheckWaitTimeMs:
			return
		self.mStart = millis()

		#self.procDbgLog("Checking board attached")

		p = subprocess.Popen(
				["st-info", "--probe"],
				stdin = subprocess.PIPE,
				stdout = subprocess.PIPE,
				stderr = subprocess.PIPE
			)

		# This acts as a function: y = f(x)
		# We can wait for the result
		out, err = p.communicate()

		if "unknown device" in out.decode("utf-8"):
			return

		self.procDbgLog("Board attached")

		# Start flashing

		# This acts as a process: y = p(x, t)
		# We can't wait for the result
		self.p = subprocess.Popen(
				[
					"st-flash",
					"--format=ihex",
					"--reset",
					"write",
					"test.hex",
				],
				stdin = subprocess.PIPE,
				stdout = subprocess.PIPE,
				stderr = subprocess.PIPE
			)

		self.procDbgLog("LED Yellow")
		yellow()

		self.mStart = millis()
		self.state = self.FlashingDoneWait

	def FlashingDoneWait(self):

		if millis() - self.mStart < CheckWaitTimeMs:
			return
		self.mStart = millis()

		res = self.p.poll()
		if res is None:
			return

		self.p.terminate()

		self.procDbgLog("Flashing done")

		if not res:
			self.procDbgLog("LED Green")
			green()
		else:
			self.procDbgLog("LED Red")
			red()

		self.mStart = millis()
		self.state = self.BoardDetachedWait

	def BoardDetachedWait(self):

		if millis() - self.mStart < CheckWaitTimeMs:
			return
		self.mStart = millis()

		#self.procDbgLog("Checking board detached")

		p = subprocess.Popen(
				["st-info", "--probe"],
				stdin = subprocess.PIPE,
				stdout = subprocess.PIPE,
				stderr = subprocess.PIPE
			)

		out, err = p.communicate()

		if not "unknown device" in out.decode("utf-8"):
			return

		self.procDbgLog("Board detached")

		self.state = self.BoardAttachedWait

if __name__ == "__main__":

	parser = argparse.ArgumentParser(description = 'Flashing Discman 4')
	#parser.add_argument('-c', '--channel', type = str, help = 'Debug channel: tty (default), socket', required = False, default = 'tty')
	#parser.add_argument('-p', '--port', type = int, help = 'Port', required = False, default = 2000)
	args = parser.parse_args()

	#aEnv.dbgChannel = args.channel
	#aEnv.dbgPort = args.port

	pApp = Dm4Flashing()

	while True:
		pApp.rootTick(None)
		sleep(0.01)

