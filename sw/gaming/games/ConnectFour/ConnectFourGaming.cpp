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

#include "ConnectFourGaming.h"

#if 1
#define dGenCfStateString(s) #s,
dProcessStateStr(CfState);
#endif

using namespace std;
using namespace Json;

string ConnectFourGaming::author = "Johannes Natter";

#define LOG_LVL	0

ConnectFourGaming::ConnectFourGaming()
	: Gaming("ConnectFourGaming")
	, mState(CfStart)
	, mGameStateChanged(false)
	, mpLobby(NULL)
{}

/* member functions */
Success ConnectFourGaming::initialize()
{
	return Positive;
}

Success ConnectFourGaming::gameProcess()
{
	gamerMsgProcess();

	switch (mState)
	{
	case CfStart:

		mState = CfLobbyStart;

		break;
	case CfTemp:

		if (!mGameStateChanged)
			break;
		mGameStateChanged = false;

		gameStateSend();

		break;
	case CfLobbyStart:

		mpLobby = ConnectFourLobbying::create();
		mpLobby->pIn = &in;
		mpLobby->pOut = &out;
		mpLobby->pGs = &mGameState;
		start(mpLobby);

		mState = CfLobbyDoneWait;

		break;
	case CfLobbyDoneWait:

		if (mpLobby->success() == Pending)
			break;

		mState = CfMatchStart;

		break;
	case CfMatchStart:

		mState = CfMatchDoneWait;

		break;
	case CfMatchDoneWait:

		mState = CfLobbyStart;

		break;
	default:
		break;
	}

	return Pending;
}

void ConnectFourGaming::gamerMsgProcess()
{
	PipeEntry<Value> msg;

	while (in.get(msg))
		gamerMsgInterpret(msg.particle);
}

void ConnectFourGaming::gamerMsgInterpret(const Value &msg)
{
	FastWriter fastWriter;
	string str = fastWriter.write(msg);
	procInfLog("%s", str.c_str());

	string type = msg["type"].asString();
	Value tmp;

	if (type == "connect")
	{
		tmp["gamerId"] = msg["gamerId"];
		tmp["gamerName"] = msg["gamerName"];

		mGameState["gamers"].append(tmp);

		mGameStateChanged = true;
		return;
	}
}

void ConnectFourGaming::gameStateSend()
{
	string frame;
	Value msg;

	msgWelcome(frame);

	msg["type"] = "frame";
	msg["data"] = frame;

	for (Value::ArrayIndex i = 0; i < mGameState["gamers"].size(); ++i)
		msg["gamers"].append(mGameState["gamers"][i]["gamerId"].asUInt64());

	out.commit(msg);
}

void ConnectFourGaming::msgWelcome(string &str)
{
	str = "\033[2J\033[H";
	str += "\r\n";
	str += "Welcome to " + mGameName + "!";
	str += "\r\n";
	str += "\r\n";
	str += "Connected gamers";
	str += "\r\n";
	for (Value::ArrayIndex i = 0; i < mGameState["gamers"].size(); ++i)
	{
		str += mGameState["gamers"][i]["gamerName"].asString();
		str += "\r\n";
	}
	str += "\r\n";
	str += "[enter]\tContinue";
	str += "\r\n";
	str += "[esc]\tExit";
	str += "\r\n";
}

void ConnectFourGaming::processInfo(char *pBuf, char *pBufEnd)
{
	dInfo("Name\t\t\t%s\n", mGameName.c_str());
#if 1
	dInfo("State\t\t\t%s\n", CfStateString[mState]);
#endif
}

/* static functions */

void ConnectFourGaming::gameInfoSet(struct TypeListElem &type)
{
	type.author = author;
}

