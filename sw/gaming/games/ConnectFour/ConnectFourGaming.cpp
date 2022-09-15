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
	, mState(CfInit)
	, mpLobby(NULL)
	, mpMatch(NULL)
{}

/* member functions */
Success ConnectFourGaming::initialize()
{
	return Positive;
}

Success ConnectFourGaming::process()
{
	switch (mState)
	{
	case CfInit:

		mGameState["name"] = mGameName;

		mState = CfLobbyStart;

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

		repel(mpLobby);
		mpLobby = NULL;

		mState = CfMatchStart;

		break;
	case CfMatchStart:

		mpMatch = ConnectFourMatching::create();
		mpMatch->pIn = &in;
		mpMatch->pOut = &out;
		mpMatch->pGs = &mGameState;
		start(mpMatch);

		mState = CfMatchDoneWait;

		break;
	case CfMatchDoneWait:

		if (mpMatch->success() == Pending)
			break;

		repel(mpMatch);
		mpMatch = NULL;

		mState = CfLobbyStart;

		break;
	default:
		break;
	}

	return Pending;
}

void ConnectFourGaming::processInfo(char *pBuf, char *pBufEnd)
{
	//dInfo("Name\t\t\t%s\n", mGameName.c_str());
#if 1
	dInfo("State\t\t\t%s\n", CfStateString[mState]);
#endif
	//dInfo("Size\t\t\t%d\n", mGameState["gamers"].size());
	string gsStr = mGameState.toStyledString();
	dInfo("%s", gsStr.c_str());
}

/* static functions */

void ConnectFourGaming::gameInfoSet(struct TypeListElem &type)
{
	type.author = author;
}

