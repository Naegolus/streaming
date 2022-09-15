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
#include "LibGaming.h"

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
	, mStart(0)
	, mCntSec(0)
{}

/* member functions */
Success ConnectFourMatching::initialize()
{
	return Positive;
}

Success ConnectFourMatching::process()
{
	Value &gs = *pGs;
	uint32_t diff = millis() - mStart;

	switch (mState)
	{
	case CfMatchInit:

		matchInit();
		gamersInit();

		mState = CfMatchBeginStart;

		break;
	case CfMatchBeginStart:

		mCntSec = 5;
		frameBeginCreate();

		mStart = millis();
		mState = CfMatchBeginCntWait;

		break;
	case CfMatchBeginCntWait:

		if (diff < 1000)
			break;
		mStart = millis();

		--mCntSec;

		if (mCntSec)
		{
			frameBeginCreate();
			break;
		}

		mState = CfMatchRoundStart;

		break;
	case CfMatchRoundStart:

		mCntSec = 5;

		if (gs["match"]["teamCurrent"] == 1)
			gs["match"]["teamCurrent"] = 2;
		else
			gs["match"]["teamCurrent"] = 1;

		mStart = millis();
		mState = CfMatchRoundDoneWait;

		break;
	case CfMatchRoundDoneWait:

		if (diff < 1000)
			break;
		mStart = millis();

		--mCntSec;

		if (mCntSec)
		{
			gs["match"]["dirty"] = true;
			break;
		}

		if (matchFinished())
		{
			mState = CfMatchStatsShow;
			break;
		}

		mState = CfMatchRoundStart;

		break;
	case CfMatchStatsShow:

		break;
	default:
		break;
	}

	return Pending;
}

void ConnectFourMatching::matchInit()
{
	Value &gs = *pGs;

	gs["match"]["teamCurrent"] = 2;
	gs["match"]["dirty"] = true;
}

void ConnectFourMatching::gamersInit()
{
	Value &gs = *pGs;

	for (Value::iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		Value &g = *iter;

		g["cursor"] = 0;
	}
}

void ConnectFourMatching::frameBeginCreate()
{
	Value &gs = *pGs;

	Value msg;
	string frame;

	msgBegin(frame);

	msg["type"] = "frame";
	msg["data"] = frame;

	for (Value::const_iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		UInt64 id = stol(iter.key().asString());
		msg["gamers"].append(id);
	}

	(*pOut).commit(msg);
}

void ConnectFourMatching::msgBegin(string &str)
{
	str = "\033[2J\033[H";
	str += "\r\n";
	str += "The game starts in " + to_string(mCntSec) + "s!";
	str += "\r\n";
}

void ConnectFourMatching::msgProcess()
{
	PipeEntry<Value> msg;

	while ((*pIn).get(msg))
		msgInterpret(msg.particle);
}

void ConnectFourMatching::msgInterpret(const Value &msg)
{
}

void ConnectFourMatching::framesRoundCreate()
{
}

bool ConnectFourMatching::matchFinished()
{
	return false;
}

void ConnectFourMatching::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", CfMatchStateString[mState]);
#endif
}

/* static functions */

