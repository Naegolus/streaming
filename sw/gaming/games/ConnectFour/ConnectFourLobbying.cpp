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
#include "LibGaming.h"

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
		gs["teams"] = objectValue;
		gs["dirty"] = true;

		mState = CflConfigSetup;

		break;
	case CflConfigSetup:

		msgProcess();
		framesCreate();

		break;
	case CflSetupDone:

		return Positive;

		break;
	default:
		break;
	}

	return Pending;
}

void ConnectFourLobbying::msgProcess()
{
	PipeEntry<Value> msg;

	while ((*pIn).get(msg))
		msgInterpret(msg.particle);
}

void ConnectFourLobbying::msgInterpret(const Value &msg)
{
	FastWriter fastWriter;
	string str = fastWriter.write(msg);
	procInfLog("%s", str.c_str());

	Value &gs = *pGs;
	string type = msg["type"].asString();
	string id = msg["gamerId"].asString();

	if (type == "connect")
	{
		Value tmp;

		if (!gs.isMember("admin"))
			gs["admin"] = id;

		tmp["name"] = msg["gamerName"];
		tmp["team"] = 0;
		tmp["dirty"] = false;

		gs["gamers"][id] = tmp;
		gs["dirty"] = true;

		return;
	}

	if (type == "disconnect")
	{
		gs["gamers"].removeMember(id);
		gs["dirty"] = true;

		return;
	}

	gamerMsgInterpret(msg);

	if (id != gs["admin"].asString())
		return;

	adminMsgInterpret(msg);
}

void ConnectFourLobbying::gamerMsgInterpret(const Value &msg)
{
	string type = msg["type"].asString();

	if (type != "key")
		return;

	Value &gs = *pGs;
	string id = msg["gamerId"].asString();
	uint8_t key = msg["key"].asUInt();

	if (keyIsNum(key))
	{
		gs["gamers"][id]["team"] = key - '0';
		gs["dirty"] = true;

		return;
	}
}

void ConnectFourLobbying::adminMsgInterpret(const Value &msg)
{
	string type = msg["type"].asString();

	if (type != "key")
		return;

	Value &gs = *pGs;
	string id = msg["gamerId"].asString();
	uint8_t key = msg["key"].asUInt();

	if (key == keyEnter)
	{
		mState = CflSetupDone;
		return;
	}
}

void ConnectFourLobbying::framesCreate()
{
	Value &gs = *pGs;

	if (!gs["dirty"].asBool())
		return;

	gs["dirty"] = false;

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
	str += "Welcome to the lobby of " + gs["name"].asString() + "!";
	str += "\r\n";
	str += "\r\n";
	str += "Team\tGamer";
	str += "\r\n";
	str += "---------------------";
	str += "\r\n";

	for (Value::iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		Value &g = *iter;

		str += g["team"].asString();
		str += "\t";
		str += g["name"].asString();
		str += "\r\n";
	}

	str += "---------------------";
	str += "\r\n";
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

