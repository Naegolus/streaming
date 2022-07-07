#!/usr/bin/python

#  This file is part of the DSP-Crowd project
#  https://www.dsp-crowd.com
#
#  Author(s):
#      - Johannes Natter, office@dsp-crowd.com
#
#  File created on 07.07.2022
#
#  Copyright (C) 2018 Authors and www.dsp-crowd.com
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

import subprocess

from Processing import *
#from libLed import *

def millis():
	return time_ns() // 10**6

CheckWaitTimeMs = 100

class Dm4Flashing(Processing):

	def initialize(self):

		self.procDbgLog("Initialized")

		self.state = self.SystemInit
		self.flashingDone = True
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
		#yellow()

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
			#green()
		else:
			self.procDbgLog("LED Red")
			#red()

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

