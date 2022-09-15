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

#ifndef CONNECT_FOUR_LOBBYING_H
#define CONNECT_FOUR_LOBBYING_H

#include <jsoncpp/json/json.h>

#include "Processing.h"
#include "Pipe.h"

#define dForEach_CflState(gen) \
		gen(CflInit) \
		gen(CflStructureInit) \
		gen(CflConfigSetup) \
		gen(CflSetupDone) \

#define dGenCflStateEnum(s) s,
dProcessStateEnum(CflState);

class ConnectFourLobbying : public Processing
{

public:

	static ConnectFourLobbying *create()
	{
		return new (std::nothrow) ConnectFourLobbying;
	}

	Pipe<Json::Value> *pIn;
	Pipe<Json::Value> *pOut;
	Json::Value *pGs;

protected:

	ConnectFourLobbying();
	virtual ~ConnectFourLobbying() {}

private:

	ConnectFourLobbying(const ConnectFourLobbying &) : Processing("") {}
	ConnectFourLobbying &operator=(const ConnectFourLobbying &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success initialize();
	Success process();
	void processInfo(char *pBuf, char *pBufEnd);

	void msgProcess();
	void msgInterpret(const Json::Value &msg);
	void gamerMsgInterpret(const Json::Value &msg);
	void adminMsgInterpret(const Json::Value &msg);

	void framesCreate();
	void msgWelcome(std::string &msg);

	/* member variables */
	enum CflState mState;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

