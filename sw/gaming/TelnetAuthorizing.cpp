/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 16.09.2022

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

#include "TelnetAuthorizing.h"
#include "LibGaming.h"

#if 1
#define dGenTaStateString(s) #s,
dProcessStateStr(TaState);
#endif

using namespace std;

#define LOG_LVL	0

TelnetAuthorizing::TelnetAuthorizing(Transfering *pConn)
	: Authorizing("TelnetAuthorizing")
	, mState(TaStart)
	, mpConn(pConn)
{}

/* member functions */
Success TelnetAuthorizing::initialize()
{
	return Positive;
}

Success TelnetAuthorizing::process()
{
	if (mpConn->success() != Pending)
		return Positive;

	string msg = "";
	uint8_t key;

	switch (mState)
	{
	case TaStart:

		mState = TaTelnetInit;

		break;
	case TaTelnetInit:

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

		mState = TaWelcomeSend;

		break;
	case TaWelcomeSend:

		msgWelcome(msg);
		mState = TaContinueWait;

		break;
	case TaContinueWait:

		key = keyGet(mpConn, mKeyLastGotMs);

		if (!key)
			break;

		if (key == keyEsc)
		{
			mAborted = true;
			return Positive;
		}

		if (key != keyEnter)
			break;

		msgName(msg);
		mState = TaNameSet;

		break;
	case TaNameSet:

		key = keyGet(mpConn, mKeyLastGotMs);

		if (!key)
			break;

		if (key == keyEsc)
		{
			mAborted = true;
			return Positive;
		}

		if (keyIsCommon(key) and mGamerName.size() < cNameSizeMax - 1)
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

		if (key != keyEnter)
			break;

		if (mGamerName.size() < cNameSizeMin or mGamerName.size() > cNameSizeMax)
			break;

		return Positive;

		break;
	default:
		break;
	}

	if (!msg.size())
		return Pending;

	mpConn->send(msg.c_str(), msg.size());

	return Pending;
}

void TelnetAuthorizing::msgWelcome(string &msg)
{
	msg = "\e[2J\e[H";
	msg += "\r\n";
	msg += "Welcome!";
	msg += "\r\n";
	msg += "\r\n";
	msg += "[enter]\tContinue";
	msg += "\r\n";
	msg += "[esc]\tQuit";
	msg += "\r\n";
}

void TelnetAuthorizing::msgName(string &msg)
{
	msg = "\e[2J\e[H";
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

void TelnetAuthorizing::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", TaStateString[mState]);
#endif
}

/* static functions */

