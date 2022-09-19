/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 16.08.2022

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

#include "GamerInteracting.h"
#include "LibGaming.h"
#include "TcpTransfering.h"
#if 0
#include "SshTransfering.h"
#include "SshAuthorizing.h"
#endif
#include "TelnetAuthorizing.h"

#if 0
#define dGenGiStateString(s) #s,
dProcessStateStr(GiState);
#endif

using namespace std;
using namespace Json;

#define LOG_LVL	0

mutex GamerInteracting::mtxGamerList;
list<GamerInteracting *> GamerInteracting::gamerList;

GamerInteracting::GamerInteracting(int fd, bool secure)
	: Processing("GamerInteracting")
	, mGamerName("")
	, mSupporter(false)
	, mpGame(NULL)
	, mState(GiStart)
	, mSocketFd(fd)
	, mConnSecure(secure)
	, mpConn(NULL)
	, mpAuth(NULL)
	, mKeyLastGotMs(0)
	, mpSelect(NULL)
{}

/* member functions */
Success GamerInteracting::initialize()
{
	return Positive;
}

Success GamerInteracting::process()
{
	if (mpConn and mpConn->success() != Pending)
		return Positive;

	string msg = "";
	Value msgGame;
	GamerInteracting *pGamer = this;

	switch (mState)
	{
	case GiStart:

		mState = GiConnStart;

		break;
	case GiConnStart:

		mpConn = TcpTransfering::create(mSocketFd);
		mpConn->procTreeDisplaySet(false);
		start(mpConn);

		mState = GiAuthStart;

		break;
	case GiAuthStart:

#if 0
		if (mConnSecure)
			mpAuth = SshAuthorizing::create(mpConn);
		else
#endif
			mpAuth = TelnetAuthorizing::create(mpConn);

		mpAuth->procTreeDisplaySet(false);
		start(mpAuth);

		mState = GiAuthDoneWait;

		break;
	case GiAuthDoneWait:

		if (mpAuth->success() == Pending)
			break;

		if (mpAuth->mAborted)
			return Positive;

		if (mpAuth->success() != Positive)
			return procErrLog(-1, "Could not authorize gamer");

		mGamerName = mpAuth->mGamerName;
		mSupporter = mpAuth->mSupporter;

		repel(mpAuth);
		mpAuth = NULL;

#if 0
		if (mConnSecure)
			mpConn = SshTransfering::create(mpConn);
#endif

		procInfLog("Continuing to server selection");
		mState = GiSelectionStart;

		break;
	case GiSelectionStart:

		mpSelect = GameSelecting::create(mpConn);
		mpSelect->procTreeDisplaySet(false);
		start(mpSelect);

		mState = GiSelectionDoneWait;

		break;
	case GiSelectionDoneWait:

		if (mpSelect->success() == Pending)
			break;

		if (mpSelect->aborted)
			return Positive;

		mpSelect->res["gamerId"] = (UInt64)pGamer;
		mpSelect->res["gamerName"] = mGamerName;

		out.commit(mpSelect->res);
		repel(mpSelect);
		mpSelect = NULL;

		mState = GiDataTransfer;

		break;
	case GiDataTransfer:

		keyProcess();
		gameMsgProcess(msg);

		break;
	default:
		break;
	}

	if (!msg.size())
		return Pending;

	mpConn->send(msg.c_str(), msg.size());

	return Pending;
}

void GamerInteracting::keyProcess()
{
	uint8_t key;

	key = keyGet(mpConn, mKeyLastGotMs);

	if (!key)
		return;

	Value msg;
	GamerInteracting *pGamer = this;

	if (key == keyEsc)
	{
		msg["type"] = "disconnect";
		msg["gamerId"] = (UInt64)pGamer;
		out.commit(msg);

		mState = GiSelectionStart;
		return;
	}

	if (!(keyIsCommon(key) or keyIsCtrl(key)))
		return;

	msg["type"] = "key";
	msg["key"] = key;
	msg["gamerId"] = (UInt64)pGamer;

	out.commit(msg);
}

void GamerInteracting::gameMsgProcess(std::string &msg)
{
	PipeEntry<Value> msgGameEntry;

	if (!in.get(msgGameEntry))
		return;

	Value msgGame = msgGameEntry.particle;
	string type = msgGame["type"].asString();

	if (type == "frame")
	{
		msg = msgGame["data"].asString();
		return;
	}

	if (type == "disconnect")
	{
		mState = GiSelectionStart;
		return;
	}
}

void GamerInteracting::processInfo(char *pBuf, char *pBufEnd)
{
	dInfo("Name\t\t\t%s\n", mGamerName.c_str());
	dInfo("Conn\t\t\t%s\n", mConnSecure ? "Secure" : "Unsecure");
#if 0
	dInfo("State\t\t\t%s\n", GiStateString[mState]);
#endif
}

/* static functions */

