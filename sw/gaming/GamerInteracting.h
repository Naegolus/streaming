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

#ifndef GAMER_INTERACTING_H
#define GAMER_INTERACTING_H

#include <list>
#include <jsoncpp/json/json.h>

#include "Processing.h"
#include "LibGaming.h"
#include "GameSelecting.h"
#include "Pipe.h"

#define dForEach_GiState(gen) \
		gen(GiStart) \
		gen(GiTerminalInit) \
		gen(GiWelcomeSend) \
		gen(GiContinueWait) \
		gen(GiNameSet) \
		gen(GiSelectionStart) \
		gen(GiSelectionDoneWait) \
		gen(GiIdle) \

#define dGenGiStateEnum(s) s,
dProcessStateEnum(GiState);

class GamerInteracting : public Processing
{

public:

	static GamerInteracting *create(int fd)
	{
		return new (std::nothrow) GamerInteracting(fd);
	}

	std::string mGamerName;

	Pipe<Json::Value> in;
	Pipe<Json::Value> out;

	static std::list<GamerInteracting *> gamerList;

protected:

	GamerInteracting(int fd);
	virtual ~GamerInteracting() {}

private:

	GamerInteracting() : Processing("GamerInteracting") {}
	GamerInteracting(const GamerInteracting &) : Processing("") {}
	GamerInteracting &operator=(const GamerInteracting &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success initialize();
	Success process();
	void processInfo(char *pBuf, char *pBufEnd);

	void msgWelcome(std::string &msg);
	void msgName(std::string &msg);
	bool keyIsAlphaNum(uint8_t key);
	bool keyIsCommon(uint8_t key);

	/* member variables */
	enum GiState mState;
	int mSocketFd;
	TcpTransfering *mpConn;
	uint32_t mKeyLastGotMs;
	GameSelecting *mpSelect;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

