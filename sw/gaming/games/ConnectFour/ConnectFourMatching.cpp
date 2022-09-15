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

#include "ConnectFourMatching.h"

#if 1
#define dGenCfMatchStateString(s) #s,
dProcessStateStr(CfMatchState);
#endif

using namespace std;
using namespace Json;

#define LOG_LVL	0

ConnectFourMatching::ConnectFourMatching()
	: Processing("ConnectFourMatching")
	, mState(CfMatchInit)
{}

/* member functions */
Success ConnectFourMatching::initialize()
{
	return Positive;
}

Success ConnectFourMatching::process()
{
	//Value &gs = *pGs;

	switch (mState)
	{
	case CfMatchInit:

		break;
	case CfMatchBeginShow:

		break;
	case CfMatchRoundStart:

		break;
	case CfMatchRoundDoneWait:

		break;
	case CfMatchStatsShow:

		break;
	default:
		break;
	}

	return Pending;
}

Success ConnectFourMatching::shutdown()
{
	return Positive;
}

void ConnectFourMatching::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", CfMatchStateString[mState]);
#endif
}

/* static functions */

