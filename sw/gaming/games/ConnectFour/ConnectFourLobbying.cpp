/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 23.08.2022

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

#include "ConnectFourLobbying.h"

#if 1
#define dGenCflStateString(s) #s,
dProcessStateStr(CflState);
#endif

using namespace std;

#define LOG_LVL	0

ConnectFourLobbying::ConnectFourLobbying()
	: Processing("ConnectFourLobbying")
	, pIn(NULL)
	, pOut(NULL)
	, pGs(NULL)
	, mState(CflStart)
{}

/* member functions */
Success ConnectFourLobbying::initialize()
{
	return Positive;
}

Success ConnectFourLobbying::process()
{
	switch (mState)
	{
	case CflStart:

		break;
	case CflTemp:

		break;
	default:
		break;
	}

	return Pending;
}

Success ConnectFourLobbying::shutdown()
{
	return Positive;
}

void ConnectFourLobbying::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", CflStateString[mState]);
#endif
}

/* static functions */

