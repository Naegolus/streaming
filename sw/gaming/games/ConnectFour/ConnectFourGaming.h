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

#ifndef CONNECT_FOUR_GAMING_H
#define CONNECT_FOUR_GAMING_H

#include "Processing.h"
#include "Gaming.h"

#define dForEach_CfState(gen) \
		gen(CfStart) \
		gen(CfTemp) \
		gen(CfLobbyStart) \
		gen(CfLobbyDoneWait) \
		gen(CfMatchStart) \
		gen(CfMatchDoneWait) \

#define dGenCfStateEnum(s) s,
dProcessStateEnum(CfState);

class ConnectFourGaming : public Gaming
{

public:

	static Gaming *create()
	{
		return new (std::nothrow) ConnectFourGaming;
	}

	static void gameInfoSet(struct TypeListElem &type);

protected:

	ConnectFourGaming();
	virtual ~ConnectFourGaming() {}

private:

	ConnectFourGaming(const ConnectFourGaming &) : Gaming("") {}
	ConnectFourGaming &operator=(const ConnectFourGaming &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success initialize();
	Success gameProcess();
	void processInfo(char *pBuf, char *pBufEnd);

	void gamerMsgProcess();
	void gamerMsgInterpret(const Json::Value &msg);

	void gameStateSend();
	void msgWelcome(std::string &msg);

	/* member variables */
	enum CfState mState;
	bool mGameStateChanged;
	Json::Value mGameState;

	/* static functions */

	/* static variables */
	static std::string author;

	/* constants */

};

#endif

