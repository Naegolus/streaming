/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 22.08.2022

  Copyright (C) 2022-now Authors and www.dsp-crowd.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <chrono>

#include "LibGaming.h"

using namespace std;
using namespace chrono;

uint8_t keyGet(Transfering *pConn, uint32_t lastGotMs)
{
	ssize_t numBytesRead;
	char buf[8];
	uint8_t key;

	numBytesRead = pConn->read(buf, sizeof(buf) - 1);
	if (!numBytesRead)
		return 0;

	buf[numBytesRead] = 0;

	char outBuf[64];
	char *pBuf = outBuf;
	char *pBufEnd = pBuf + sizeof(outBuf);

	*pBuf = 0;
	for (ssize_t i = 0; i < numBytesRead; ++i)
		dInfo(" 0x%02X", buf[i] & 0xFF);

	if (buf[0] == 0x0D)
		numBytesRead = 1;

	if (numBytesRead >= 2)
	{
		wrnLog("data received, %d:%s", numBytesRead, outBuf);
		return 0;
	}

	key = buf[0];

#if 1
	infLog("key received: 0x%02X '%c'", key, key);
#endif

	return key;
}

bool keyIsAlphaNum(uint8_t key)
{
	if (key >= 'a' and key <= 'z')
		return true;

	if (key >= 'A' and key <= 'Z')
		return true;

	if (keyIsNum(key))
		return true;

	return false;
}

bool keyIsNum(uint8_t key)
{
	if (key >= '0' and key <= '9')
		return true;

	return false;
}

bool keyIsCommon(uint8_t key)
{
	if (keyIsAlphaNum(key))
		return true;

	if (key == ' ' or key == '-' or key == '_')
		return true;

	return false;
}

bool keyIsCtrl(uint8_t key)
{
	if (key == keyEnter or key == keyEsc)
		return true;

	return false;
}

uint32_t millis()
{
	auto now = steady_clock::now();
	auto nowMs = time_point_cast<milliseconds>(now);
	return nowMs.time_since_epoch().count();
}

