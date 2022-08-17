/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 16.08.2022

  Copyright (C) 2022 Authors and www.dsp-crowd.com

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

#include "TicTacToeGaming.h"
#include "GamerInteracting.h"

using namespace std;

#define LOG_LVL	0

typedef list<struct GamingPeer>::iterator PeerIter;

TicTacToeGaming::TicTacToeGaming()
	: Processing("TicTacToeGaming")
	, mpLst(NULL)
{}

/* member functions */
Success TicTacToeGaming::initialize()
{
	mpLst = TcpListening::create();
	mpLst->portSet(4001);
	start(mpLst);

	return Positive;
}

Success TicTacToeGaming::process()
{
	peerListUpdate();

	return Pending;
}

void TicTacToeGaming::peerListUpdate()
{
	peerRemove();
	peerAdd(mpLst, GamingPeerStd, "Std");
}

void TicTacToeGaming::peerRemove()
{
	PeerIter iter;
	struct GamingPeer peer;
	Processing *pProc;

	iter = mPeerList.begin();
	while (iter != mPeerList.end())
	{
		peer = *iter;
		pProc = peer.pProc;

		if (pProc->success() == Pending)
		{
			++iter;
			continue;
		}

		procDbgLog(LOG_LVL, "removing %s peer. process: %p", peer.typeDesc.c_str(), pProc);
		repel(pProc);

		iter = mPeerList.erase(iter);
	}
}

void TicTacToeGaming::peerAdd(TcpListening *pListener, enum GamingPeerType peerType, const char *pTypeDesc)
{
	int peerFd;
	Processing *pProc = NULL;
	struct GamingPeer peer;

	while (1)
	{
		if (pListener->ppPeerFd.isEmpty())
			break;

		peerFd = pListener->ppPeerFd.front();
		pListener->ppPeerFd.pop();

		pProc = GamerInteracting::create(peerFd);
		start(pProc);

		procDbgLog(LOG_LVL, "adding %s peer. process: %p", pTypeDesc, pProc);

		peer.type = peerType;
		peer.typeDesc = pTypeDesc;
		peer.pProc = pProc;

		mPeerList.push_back(peer);
	}
}

void TicTacToeGaming::processInfo(char *pBuf, char *pBufEnd)
{
	(void)pBuf;
	(void)pBufEnd;
}

/* static functions */

