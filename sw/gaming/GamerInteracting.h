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

#ifndef GAMER_INTERACTING_H
#define GAMER_INTERACTING_H

#include <list>
#include <jsoncpp/json/json.h>

#include "Processing.h"
#include "Pipe.h"
#include "GameSelecting.h"
#include "Authorizing.h"

#define dForEach_GiState(gen) \
		gen(GiStart) \
		gen(GiConnStart) \
		gen(GiAuthStart) \
		gen(GiAuthDoneWait) \
		gen(GiSelectionStart) \
		gen(GiSelectionDoneWait) \
		gen(GiDataTransfer) \

#define dGenGiStateEnum(s) s,
dProcessStateEnum(GiState);

class Gaming;

class GamerInteracting : public Processing
{

public:

	static GamerInteracting *create(int fd, bool secure = false)
	{
		return new (std::nothrow) GamerInteracting(fd, secure);
	}

	std::string mGamerName;
	bool mSupporter;

	Pipe<Json::Value> in;
	Pipe<Json::Value> out;

	Gaming *mpGame;

	static std::mutex mtxGamerList;
	static std::list<GamerInteracting *> gamerList;

protected:

	GamerInteracting(int fd, bool secure);
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

	void keyProcess();
	void gameMsgProcess(std::string &msg);

	/* member variables */
	enum GiState mState;
	int mSocketFd;
	bool mConnSecure;
	Transfering *mpConn;
	Authorizing *mpAuth;
	uint32_t mKeyLastGotMs;
	GameSelecting *mpSelect;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

