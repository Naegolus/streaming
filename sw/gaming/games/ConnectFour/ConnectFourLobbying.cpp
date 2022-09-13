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
using namespace Json;

#define LOG_LVL	0

ConnectFourLobbying::ConnectFourLobbying()
	: Processing("ConnectFourLobbying")
	, pIn(NULL)
	, pOut(NULL)
	, pGs(NULL)
	, mState(CflStart)
	, mStructureDone(false)
	, mGameStateChanged(false)
{}

/* member functions */
Success ConnectFourLobbying::initialize()
{
	return Positive;
}

Success ConnectFourLobbying::process()
{
	Value &gs = *pGs;

	switch (mState)
	{
	case CflStart:

		mState = CflStructureInit;

		break;
	case CflStructureInit:

		gs["gamers"] = objectValue;
		gs["teams"] = arrayValue;

		mStructureDone = true;
		mState = CflConfigSetup;

		break;
	case CflConfigSetup:

		gamerMsgProcess();
		framesSend();

		break;
	case CflSetupDone:

		return Positive;

		break;
	default:
		break;
	}

	return Pending;
}

void ConnectFourLobbying::gamerMsgProcess()
{
	if (!mStructureDone)
		return;

	PipeEntry<Value> msg;

	while ((*pIn).get(msg))
		gamerMsgInterpret(msg.particle);
}

void ConnectFourLobbying::gamerMsgInterpret(const Value &msg)
{
	FastWriter fastWriter;
	string str = fastWriter.write(msg);
	procInfLog("%s", str.c_str());

	string type = msg["type"].asString();
	string id = msg["gamerId"].asString();
	Value tmp;

	if (type == "connect")
	{
		tmp["gamerName"] = msg["gamerName"];

		(*pGs)["gamers"][id] = tmp;

		mGameStateChanged = true;
		return;
	}
}

void ConnectFourLobbying::framesSend()
{
	if (!mGameStateChanged)
		return;

	mGameStateChanged = false;

	Value &gs = *pGs;
	Value msg;
	string frame;

	msgWelcome(frame);

	msg["type"] = "frame";
	msg["data"] = frame;

	for (Value::const_iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		UInt64 id = stol(iter.key().asString());
		msg["gamers"].append(id);
	}

	(*pOut).commit(msg);
}

void ConnectFourLobbying::msgWelcome(string &str)
{
	Value &gs = *pGs;

	str = "\033[2J\033[H";
	str += "\r\n";
	str += "Welcome to " + gs["gameName"].asString() + "!";
	str += "\r\n";
	str += "\r\n";
	str += "Connected gamers";
	str += "\r\n";
	for (Value::const_iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		str += (*iter)["gamerName"].asString();
		str += "\r\n";
	}
	str += "\r\n";
	str += "[esc]\tExit";
	str += "\r\n";
}

void ConnectFourLobbying::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", CflStateString[mState]);
#endif
}

/* static functions */

