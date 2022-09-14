/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 20.08.2022

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

#include "GameSelecting.h"
#include "Gaming.h"
#include "LibGaming.h"

#if 1
#define dGenGsStateString(s) #s,
dProcessStateStr(GsState);
#endif

using namespace std;
using namespace Json;

#define LOG_LVL	0

#define dNameColSize		20
#define dGameRowSize		5

#define dTypeRowSize		7
#define dNameSizeMax		16

GameSelecting::GameSelecting(TcpTransfering *pConn)
	: Processing("GameSelecting")
	, aborted(false)
	, mState(GsStart)
	, mpConn(pConn)
	, mKeyLastGotMs(0)
	, mNumGames(0)
	, mNumTypes(0)
	, mNumGamers(0)
	, mOffGamesCursor(0)
	, mOffGames(0)
	, mOffTypesCursor(0)
	, mOffTypes(0)
	, mGameName("")
{}

/* member functions */
Success GameSelecting::initialize()
{
	return Positive;
}

Success GameSelecting::process()
{
	string msg = "";
	uint8_t key;

	switch (mState)
	{
	case GsStart:

		mNumTypes = Gaming::typesList.size();

		mState = GsGamesListRead;

		break;
	case GsGamesListRead:

		procInfLog("Getting server list");

		mGamesList.clear();

		{
			lock_guard<mutex> lock(Gaming::mtxGamesList);
			list<Gaming *>::iterator iter;
			Gaming *pGaming = NULL;
			GameListElem game;

			mNumGames = Gaming::gamesList.size();
			mGamesList.reserve(mNumGames);

			iter = Gaming::gamesList.begin();
			for (; iter != Gaming::gamesList.end(); ++iter)
			{
				pGaming = *iter;

				game.id = pGaming;
				game.name = pGaming->mGameName;
				game.type = pGaming->mType;

				mGamesList.push_back(game);
			}
		}

		mOffGamesCursor = 0;
		mOffGames = 0;

		procInfLog("Got server list");

		msgGamesList(msg);
		mState = GsGamesList;

		break;
	case GsGamesList:

		key = keyGet(mpConn, mKeyLastGotMs);

		if (!key)
			break;

		if (key == keyEsc)
		{
			aborted = true;
			return Positive;
		}

		if (key == 'j' and mOffGamesCursor < dGameRowSize - 1 and mOffGamesCursor < mNumGames - 1)
		{
			if ((mOffGamesCursor == dGameRowSize / 2) and
				(mOffGames + dGameRowSize < mNumGames))
				++mOffGames;
			else
				++mOffGamesCursor;

			msgGamesList(msg);
			break;
		}

		if (key == 'k' and mOffGamesCursor)
		{
			if ((mOffGamesCursor == dGameRowSize / 2) and mOffGames)
				--mOffGames;
			else
				--mOffGamesCursor;

			msgGamesList(msg);
			break;
		}

		if (key == 'c')
		{
			msgTypesList(msg);

			mOffTypesCursor = 0;
			mOffTypes = 0;

			mState = GsTypesList;
			break;
		}

		if (key == 'r')
		{
			mState = GsGamesListRead;
			break;
		}

		if (key != keyEnter)
			break;

		if (!mGamesList.size())
			break;

		res["type"] = "connect";
		res["gameId"] = (UInt64)mGamesList[mOffGamesCursor + mOffGames].id;

		return Positive;

		break;
	case GsTypesList:

		key = keyGet(mpConn, mKeyLastGotMs);

		if (!key)
			break;

		if (key == keyEsc)
		{
			msgGamesList(msg);
			mState = GsGamesList;
			break;
		}

		if (key == 'j' and mOffTypesCursor < dTypeRowSize - 1 and mOffTypesCursor < mNumTypes - 1)
		{
			if ((mOffTypesCursor == dTypeRowSize / 2) and
				(mOffTypes + dTypeRowSize < mNumTypes))
				++mOffTypes;
			else
				++mOffTypesCursor;

			msgTypesList(msg);
			break;
		}

		if (key == 'k' and mOffTypesCursor)
		{
			if ((mOffTypesCursor == dTypeRowSize / 2) and mOffTypes)
				--mOffTypes;
			else
				--mOffTypesCursor;

			msgTypesList(msg);
			break;
		}

		if (key != keyEnter)
			break;

		res["type"] = "create";
		res["gameType"] = Gaming::typesList[mOffTypes + mOffTypesCursor].name;

		msgName(msg);
		mState = GsNameSet;

		break;
	case GsNameSet:

		key = keyGet(mpConn, mKeyLastGotMs);

		if (!key)
			break;

		if (key == keyEsc)
		{
			msgTypesList(msg);

			mOffTypesCursor = 0;
			mOffTypes = 0;

			mState = GsTypesList;
			break;
		}

		if (keyIsCommon(key) and mGameName.size() < cNameSizeMax - 1)
		{
			mGameName.push_back(key);
			msgName(msg);
			break;
		}

		if (key == keyBackspace and mGameName.size())
		{
			mGameName.pop_back();
			msgName(msg);
			break;
		}

		if (key != keyEnter)
			break;

		if (mGameName.size() < cNameSizeMin or mGameName.size() > cNameSizeMax)
			break;

		res["gameName"] = mGameName;

		return Positive;
	default:
		break;
	}

	if (!msg.size())
		return Pending;

	mpConn->send(msg.c_str(), msg.size());

	return Pending;
}

void GameSelecting::msgGamesList(string &msg)
{
	struct GameListElem *pElem = NULL;
	size_t u = mOffGames;
	size_t i = 0;
	string str;

	msg = "\033[2J\033[H";
	msg += "\r\n";

	msg += "Games " + to_string(mNumGames) + ", Gamers " + to_string(mNumGamers) + "\r\n";
	msg += "\r\n";

	str = "  Name";
	str.insert(str.size(), dNameColSize - str.size(), ' ');
	msg += str + "Type\r\n";
	msg += "-----------------------------------\r\n";

	for (i = 0; i < dGameRowSize; ++i, ++u)
	{
		if ((!i and mOffGames) or
			(i == dGameRowSize - 1 and mOffGames + dGameRowSize < mNumGames))
		{
			msg += "  ---\r\n";
			continue;
		}

		if (u >= mGamesList.size())
		{
			msg += "\r\n";
			continue;
		}

		pElem = &mGamesList[u];

		str = "";
		if (i == mOffGamesCursor)
			str += ">";
		else
			str += " ";
		str += " ";

		str += pElem->name;
		if (str.size() < dNameColSize)
			str.insert(str.size(), dNameColSize - str.size(), ' ');

		msg += str + pElem->type + "\r\n";
	}

	msg += "-----------------------------------\r\n";
	msg += "\r\n";
	msg += "[k]\t\tUp\r\n";
	msg += "[j]\t\tDown\r\n";
	msg += "[enter]\t\tSelect\r\n";
	msg += "[c]\t\tCreate\r\n";
	msg += "[r]\t\tRefresh\r\n";
	msg += "[esc]\t\tQuit\r\n";
}

void GameSelecting::msgTypesList(string &msg)
{
	struct TypeListElem *pElem = NULL;
	size_t u = mOffTypes;
	size_t i = 0;
	string str;

	msg = "\033[2J\033[H";
	msg += "\r\n";

	msg += "Available Games\r\n";
	msg += "\r\n";

	for (i = 0; i < dTypeRowSize; ++i, ++u, ++pElem)
	{
		if ((!i and mOffTypes) or
			(i == dTypeRowSize - 1 and mOffTypes + dTypeRowSize < mNumTypes))
		{
			msg += "  ---" + string(dNameColSize - 2, ' ') + "|\r\n";
			continue;
		}

		if (u >= mNumTypes)
		{
			msg += "\r\n";
			continue;
		}

		pElem = &Gaming::typesList[u];

		if (i == mOffTypesCursor)
			msg += ">";
		else
			msg += " ";

		str = pElem->name;
		if (str.size() < dNameColSize)
			str.insert(str.size(), dNameColSize - str.size(), ' ');

		msg += " " + str + " |\r\n";
	}

	msg += "\r\n";
	msg += "[k]\t\tUp\r\n";
	msg += "[j]\t\tDown\r\n";
	msg += "[enter]\t\tSelect\r\n";
	msg += "[esc]\t\tReturn\r\n";
}

void GameSelecting::msgName(std::string &msg)
{
	msg = "\033[2J\033[H";
	msg += "\r\n";
	msg += "Set game name!";
	msg += "\r\n";
	msg += "\r\n";
	msg += "Name: ";
	msg += mGameName;
	msg += "\r\n";
	msg += "\r\n";
	msg += "[enter]\tContinue";
	msg += "\r\n";
	msg += "[esc]\tReturn";
	msg += "\r\n";
	msg += "\r\n";
	msg += "Requirements: 1 < len < 16";
	msg += "\r\n";
}

void GameSelecting::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", GsStateString[mState]);
#endif
}

/* static functions */

