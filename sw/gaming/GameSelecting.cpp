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
#include "GamerInteracting.h"
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

GameSelecting::GameSelecting(Transfering *pConn)
	: Processing("GameSelecting")
	, aborted(false)
	, mState(GsStart)
	, mpConn(pConn)
	, mKeyLastGotMs(0)
	, mNumGamers(0)
	, mIdxGames(dGameRowSize)
	, mIdxTypes(dTypeRowSize)
	, mGameName("")
{}

/* member functions */
Success GameSelecting::initialize()
{
	return Positive;
}

Success GameSelecting::process()
{
	if (mpConn and mpConn->success() != Pending)
		return Positive;

	string msg = "";
	uint8_t key;

	switch (mState)
	{
	case GsStart:

		mIdxTypes = Gaming::typesList.size();

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

			mIdxGames = Gaming::gamesList.size();
			mGamesList.reserve(mIdxGames.size());

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

		{
			lock_guard<mutex> lock(GamerInteracting::mtxGamerList);
			mNumGamers = GamerInteracting::gamerList.size();
		}

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

		if (key == 'j' and mIdxGames.inc())
		{
			msgGamesList(msg);
			break;
		}

		if (key == 'k' and mIdxGames.dec())
		{
			msgGamesList(msg);
			break;
		}

		if (key == 'c')
		{
			msgTypesList(msg);
			mIdxTypes.reset();

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
		res["gameId"] = (UInt64)mGamesList[mIdxGames.cursorAbs()].id;

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

		if (key == 'j' and mIdxTypes.inc())
		{
			msgTypesList(msg);
			break;
		}

		if (key == 'k' and mIdxTypes.dec())
		{
			msgTypesList(msg);
			break;
		}

		if (key != keyEnter)
			break;

		res["type"] = "create";
		res["gameType"] = Gaming::typesList[mIdxTypes.cursorAbs()].name;

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
			mIdxTypes.reset();

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
	size_t u = mIdxGames.offset();
	size_t i = 0;
	string str;

	msg = "\e[2J\e[H";
	msg += "\r\n";

	msg += "Games " + to_string(mIdxGames.size()) + ", Gamers " + to_string(mNumGamers) + "\r\n";
	msg += "\r\n";

	str = "  Name";
	str.insert(str.size(), dNameColSize - str.size(), ' ');
	msg += str + "Type\r\n";
	msg += "-----------------------------------\r\n";

	for (i = 0; i < mIdxGames.win(); ++i, ++u)
	{
		if ((!i and mIdxGames.offset()) or
			(i == mIdxGames.win() - 1 and !mIdxGames.endReached()))
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
		if (i == mIdxGames.cursor())
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
	size_t u = mIdxTypes.offset();
	size_t i = 0;
	string str;

	msg = "\e[2J\e[H";
	msg += "\r\n";

	msg += "Available Games\r\n";
	msg += "\r\n";

	for (i = 0; i < mIdxTypes.win(); ++i, ++u, ++pElem)
	{
		if ((!i and mIdxTypes.offset()) or
			(i == mIdxTypes.win() - 1 and !mIdxTypes.endReached()))
		{
			msg += "  ---" + string(dNameColSize - 2, ' ') + "|\r\n";
			continue;
		}

		if (u >= mIdxTypes.size())
		{
			msg += "\r\n";
			continue;
		}

		pElem = &Gaming::typesList[u];

		if (i == mIdxTypes.cursor())
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
	msg = "\e[2J\e[H";
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

