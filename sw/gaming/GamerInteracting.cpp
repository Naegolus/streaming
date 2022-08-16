/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 16.08.2022

  Copyright (C) 2022 Authors and www.dsp-crowd.com

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

#include "GamerInteracting.h"

using namespace std;

#define LOG_LVL	0

GamerInteracting::GamerInteracting(int fd)
	: Processing("GamerInteracting")
	, mSocketFd(fd)
{}

/* member functions */
Success GamerInteracting::initialize()
{
	mpConn = TcpTransfering::create(mSocketFd);
	start(mpConn);

	return Positive;
}

Success GamerInteracting::process()
{
	if (mpConn->success() != Pending)
		return Positive;

	dataRead();

	return Pending;
}

void GamerInteracting::dataRead()
{
	ssize_t numBytesRead;
	char buf[8];

	numBytesRead = mpConn->read(buf, sizeof(buf) - 1);
	if (!numBytesRead)
		return;

	buf[numBytesRead] = 0;

	procWrnLog("data received, %d: %s", numBytesRead, buf);
}

void GamerInteracting::processInfo(char *pBuf, char *pBufEnd)
{
	(void)pBuf;
	(void)pBufEnd;
}

/* static functions */

