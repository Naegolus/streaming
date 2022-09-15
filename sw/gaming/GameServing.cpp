/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 17.08.2022

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

#include "GameServing.h"
#include "Gaming.h"
#include "TicTacToeGaming.h"
#include "ConnectFourGaming.h"

using namespace std;
using namespace Json;

#define LOG_LVL	0

typedef list<GamerInteracting *>::iterator GamerIter;
typedef list<Gaming *>::iterator GameIter;

GameServing::GameServing()
	: Processing("GameServing")
	, mpLst(NULL)
{}

/* member functions */
Success GameServing::initialize()
{
	dGameRegister(ConnectFour);
	dGameRegister(TicTacToe);
#if 0
	dGameRegister(TicTacToe);
	dGameRegister(ConnectFour);
	dGameRegister(ConnectFour);
	dGameRegister(ConnectFour);
	dGameRegister(TicTacToe);
	dGameRegister(ConnectFour);
	dGameRegister(ConnectFour);
	dGameRegister(TicTacToe);
	dGameRegister(TicTacToe);
	dGameRegister(ConnectFour);
#endif
	// dGameRegister(Battleship);

	mpLst = TcpListening::create();
	mpLst->portSet(4000);
	mpLst->procTreeDisplaySet(false);
	start(mpLst);

	return Positive;
}

Success GameServing::process()
{
	gamerListUpdate();

	gamerMsgProcess();
	gameMsgProcess();

	return Pending;
}

void GameServing::gamerMsgProcess()
{
	GamerIter iter;
	GamerInteracting *pGamer = NULL;
	PipeEntry<Value> msg;

	iter = GamerInteracting::gamerList.begin();
	for (; iter != GamerInteracting::gamerList.end(); ++iter)
	{
		pGamer = *iter;

		while (pGamer->out.get(msg))
			gamerMsgInterpret(pGamer, msg.particle);
	}
}

void GameServing::gamerMsgInterpret(GamerInteracting *pGamer, const Value &msg)
{
	FastWriter fastWriter;
	string str = fastWriter.write(msg);
	procInfLog("%s", str.c_str());

	string type = msg["type"].asString();
	Gaming *pGame = NULL;
	Value gameMsg;

	if (type == "create")
	{
		pGame = Gaming::create(msg["gameType"].asString());
		if (!pGame)
			return;

		pGame->mGameName = msg["gameName"].asString();
		start(pGame);

		{
			lock_guard<mutex> lock(Gaming::mtxGamesList);
			Gaming::gamesList.push_back(pGame);
		}

		pGamer->mpGame = pGame;

		gameMsg["type"] = "connect";
		gameMsg["gamerId"] = (UInt64)pGamer;
		gameMsg["gamerName"] = pGamer->mGamerName;

		pGame->in.commit(gameMsg);
		return;
	}

	if (type == "connect")
	{
		// TODO: Check if game available

		pGame = (Gaming *)msg["gameId"].asUInt64();

		pGamer->mpGame = pGame;
		pGame->in.commit(msg);
		return;
	}

	if (type == "disconnect")
	{
		pGamer->mpGame->in.commit(msg);
		pGamer->mpGame = NULL;
		return;
	}

	if (type == "key")
	{
		pGamer->mpGame->in.commit(msg);
		return;
	}
}

void GameServing::gameMsgProcess()
{
	GameIter iter;
	Gaming *pGame = NULL;
	PipeEntry<Value> msg;

	iter = Gaming::gamesList.begin();
	for (; iter != Gaming::gamesList.end(); ++iter)
	{
		pGame = *iter;

		while (pGame->out.get(msg))
			gameMsgInterpret(pGame, msg.particle);
	}
}

void GameServing::gameMsgInterpret(Gaming *pGame, Value &msg)
{
	FastWriter fastWriter;
	string str = fastWriter.write(msg);
	procInfLog("%s", str.c_str());

	string type = msg["type"].asString();

	if (type == "frame")
	{
		frameDispatch(msg);
		return;
	}

	if (type == "disconnect")
	{
		GamerInteracting *pGamer = NULL;
		pGamer = (GamerInteracting *)msg["gamerId"].asUInt64();
		pGamer->in.commit(msg);
		return;
	}
}

void GameServing::frameDispatch(Value &msg)
{
	if (!msg.isMember("gamers"))
		return;

	Value gamers = msg["gamers"];
	msg["gamers"] = "";
	GamerInteracting *pGamer = NULL;

	for (Value::ArrayIndex i = 0; i < gamers.size(); ++i)
	{
		pGamer = (GamerInteracting *)gamers[i].asUInt64();
		pGamer->in.commit(msg);
	}
}

void GameServing::gamerListUpdate()
{
	gamerRemove();
	gamerAdd();
}

void GameServing::gamerRemove()
{
	GamerIter iter;
	GamerInteracting *pGamer;

	iter = GamerInteracting::gamerList.begin();
	while (iter != GamerInteracting::gamerList.end())
	{
		pGamer = *iter;

		if (pGamer->success() == Pending)
		{
			++iter;
			continue;
		}

		procDbgLog(LOG_LVL, "removing gamer. process: %p", pGamer);
		repel(pGamer);

		{
			lock_guard<mutex> lock(GamerInteracting::mtxGamerList);
			iter = GamerInteracting::gamerList.erase(iter);
		}
	}
}

void GameServing::gamerAdd()
{
	PipeEntry<int> peerFdEntry;
	int peerFd;
	GamerInteracting *pGamer = NULL;

	while (1)
	{
		if (!mpLst->ppPeerFd.get(peerFdEntry))
			break;

		peerFd = peerFdEntry.particle;

		pGamer = GamerInteracting::create(peerFd);
		pGamer->procTreeDisplaySet(false);
		start(pGamer);

		procDbgLog(LOG_LVL, "adding gamer. process: %p", pGamer);

		{
			lock_guard<mutex> lock(GamerInteracting::mtxGamerList);
			GamerInteracting::gamerList.push_back(pGamer);
		}
	}
}

void GameServing::processInfo(char *pBuf, char *pBufEnd)
{
	dInfo("Gamers\t\t\t%ld", GamerInteracting::gamerList.size());
}

/* static functions */

