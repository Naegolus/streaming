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

#if 0
#define dGenGiStateString(s) #s,
dProcessStateStr(GiState);
#endif

using namespace std;

#define LOG_LVL	0

#define dNameSizeMax		16

list<GamerInteracting *> GamerInteracting::gamerList;

GamerInteracting::GamerInteracting(int fd)
	: Processing("GamerInteracting")
	, mGamerName("")
	, mState(GiStart)
	, mSocketFd(fd)
	, mpConn(NULL)
	, mKeyLastGotMs(0)
	, mpSelect(NULL)
{}

/* member functions */
Success GamerInteracting::initialize()
{
	mpConn = TcpTransfering::create(mSocketFd);
	mpConn->procTreeDisplaySet(false);
	start(mpConn);

	return Positive;
}

Success GamerInteracting::process()
{
	if (mpConn->success() != Pending)
		return Positive;

	string msg = "";
	uint8_t key;

	switch (mState)
	{
	case GiStart:

		mState = GiTerminalInit;

		break;
	case GiTerminalInit:

		// IAC WILL ECHO
		msg += "\xFF\xFB\x01";

		// IAC WILL SUPPRESS_GO_AHEAD
		msg += "\xFF\xFB\x03";

		// IAC WONT LINEMODE
		msg += "\xFF\xFC\x22";

		// Hide cursor
		msg += "\e[?25l";

		// Set terminal title
		msg += "\e]2;twitch.tv/Naegolus\a";

		mState = GiWelcomeSend;

		break;
	case GiWelcomeSend:

		msgWelcome(msg);

		mState = GiContinueWait;

		break;
	case GiContinueWait:

		key = keyGet(mpConn, mKeyLastGotMs);

		if (!key)
			break;

		if (key == keyEsc)
			return Positive;

		if (key != keyEnter)
			break;

		msgName(msg);

		mState = GiNameSet;

		break;
	case GiNameSet:

		key = keyGet(mpConn, mKeyLastGotMs);

		if (!key)
			break;

		if (key == keyEsc)
			return Positive;

		if (keyIsCommon(key) and mGamerName.size() < dNameSizeMax - 1)
		{
			mGamerName.push_back(key);
			msgName(msg);
			break;
		}

		if (key == keyBackspace and mGamerName.size())
		{
			mGamerName.pop_back();
			msgName(msg);
			break;
		}

		if (key == keyEnter and 1 < mGamerName.size() and mGamerName.size() < dNameSizeMax)
		{
			procInfLog("Continuing to server selection");
			mState = GiSelectionStart;
			break;
		}

		break;
	case GiSelectionStart:

		mpSelect = GameSelecting::create(mpConn);
		//mpSelect->procTreeDisplaySet(false);
		start(mpSelect);

		mState = GiSelectionDoneWait;

		break;
	case GiSelectionDoneWait:

		if (mpSelect->success() == Pending)
			break;

		if (mpSelect->aborted)
			return Positive;

		out.commit(mpSelect->res);
		repel(mpSelect);
		mpSelect = NULL;

		mState = GiIdle;

		break;
	case GiIdle:

		break;
	default:
		break;
	}

	if (!msg.size())
		return Pending;

	mpConn->send(msg.c_str(), msg.size());

	return Pending;
}

void GamerInteracting::msgWelcome(string &msg)
{
	msg = "\033[2J\033[H";
	msg += "\r\n";
	msg += "Welcome!";
	msg += "\r\n";
	msg += "\r\n";
	msg += "[enter]\tContinue";
	msg += "\r\n";
	msg += "[esc]\tQuit";
	msg += "\r\n";
}

void GamerInteracting::msgName(string &msg)
{
	msg = "\033[2J\033[H";
	msg += "\r\n";
	msg += "Set your name warrior!";
	msg += "\r\n";
	msg += "\r\n";
	msg += "Name: ";
	msg += mGamerName;
	msg += "\r\n";
	msg += "\r\n";
	msg += "[enter]\tContinue";
	msg += "\r\n";
	msg += "[esc]\tQuit";
	msg += "\r\n";
	msg += "\r\n";
	msg += "Requirements: 1 < len < 16";
	msg += "\r\n";
}

void GamerInteracting::processInfo(char *pBuf, char *pBufEnd)
{
	dInfo("Name\t\t\t%s\n", mGamerName.c_str());
#if 0
	dInfo("State\t\t\t%s\n", GiStateString[mState]);
#endif
}

/* static functions */

