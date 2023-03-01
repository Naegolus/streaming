#!/usr/bin/python

#  This file is part of the DSP-Crowd project
#  https://www.dsp-crowd.com
#
#  Author(s):
#      - Johannes Natter, office@dsp-crowd.com
#
#  File created on 07.07.2022
#
#  Copyright (C) 2022-now Authors and www.dsp-crowd.com
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

fileHex = ""

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
		try:
			out, err = p.communicate(timeout = 1)
		except TimeoutExpired:
			p.kill()
			out, err = p.communicate()

		self.programmerOkCheck()

		if not "G030/G031/G041" in out.decode("utf-8"):
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
					fileHex,
				],
				stdin = subprocess.PIPE,
				stdout = subprocess.PIPE,
				stderr = subprocess.PIPE
			)

		self.procDbgLog("Flashing")
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

		try:
			out, err = self.p.communicate(timeout = 1)
		except TimeoutExpired:
			p.kill()
			out, err = self.p.communicate()

		self.p.terminate()

		self.procDbgLog("Flashing done")

		if res:
			red()
			self.procDbgLog("Failed")
			print(err.decode("utf-8"))

			self.mStart = millis()
			self.state = self.BoardDetachedWait

			return

		green()
		self.procDbgLog("Success")

		p = subprocess.Popen(
				[
				"st-flash",
				"--area=option",
				"write", "0xdfffe1ab",
				],
				stdin = subprocess.PIPE,
				stdout = subprocess.PIPE,
				stderr = subprocess.PIPE
			)

		try:
			out, err = p.communicate(timeout = 1)
		except TimeoutExpired:
			p.kill()
			out, err = p.communicate()

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

		try:
			out, err = p.communicate(timeout = 1)
		except TimeoutExpired:
			p.kill()
			out, err = p.communicate()

		if "G030/G031/G041" in out.decode("utf-8"):
			return

		self.procDbgLog("Board detached")

		self.state = self.BoardAttachedWait

	def programmerOkCheck(self):

		p = subprocess.Popen(
				["st-info", "--probe"],
				stdin = subprocess.PIPE,
				stdout = subprocess.PIPE,
				stderr = subprocess.PIPE
			)

		try:
			out, err = p.communicate(timeout = 1)
		except TimeoutExpired:
			p.kill()
			out, err = p.communicate()

		if not "Found 0" in out.decode("utf-8"):
			#self.procDbgLog("Everything is awesome")
			return

		self.procDbgLog("could not find the ST-Link programmer")

		self.procDbgLog("trying to reset USB device")

		p = subprocess.Popen(
				[
				"usb_modeswitch",
				"-v", "0x0483",
				"-p", "0x3748",
				"--reset-usb",
				],
				stdin = subprocess.PIPE,
				stdout = subprocess.PIPE,
				stderr = subprocess.PIPE
			)

		try:
			out, err = p.communicate(timeout = 4)
		except TimeoutExpired:
			p.kill()
			out, err = p.communicate()

		self.procDbgLog("trying to reset USB device: done")

		pass

if __name__ == "__main__":

	parser = argparse.ArgumentParser(description = 'Flashing Discman 4')
	parser.add_argument('-f', '--fileHex', type = str, help = 'HEX file to download to uC', required = False, default = '/home/pi/production.hex')
	#parser.add_argument('-p', '--port', type = int, help = 'Port', required = False, default = 2000)
	args = parser.parse_args()

	fileHex = args.fileHex
	#aEnv.dbgPort = args.port

	pApp = Dm4Flashing()

	while True:
		pApp.rootTick(None)
		sleep(0.01)

