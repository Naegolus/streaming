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

#include "LibGaming.h"

using namespace std;

uint8_t keyGet(TcpTransfering *pConn, uint32_t lastGotMs)
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

	infLog("key received: 0x%02X '%c'", key, key);

	return key;
}

