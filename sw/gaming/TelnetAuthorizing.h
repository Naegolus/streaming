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

#ifndef TELNET_AUTHORIZING_H
#define TELNET_AUTHORIZING_H

#include "Authorizing.h"
#include "Transfering.h"

#define dForEach_TaState(gen) \
		gen(TaStart) \
		gen(TaTelnetInit) \
		gen(TaWelcomeSend) \
		gen(TaContinueWait) \
		gen(TaNameSet) \

#define dGenTaStateEnum(s) s,
dProcessStateEnum(TaState);

class TelnetAuthorizing : public Authorizing
{

public:

	static TelnetAuthorizing *create(Transfering *pConn)
	{
		return new (std::nothrow) TelnetAuthorizing(pConn);
	}

protected:

	TelnetAuthorizing(Transfering *pConn);
	virtual ~TelnetAuthorizing() {}

private:

	TelnetAuthorizing() : Authorizing("") {}
	TelnetAuthorizing(const TelnetAuthorizing &) : Authorizing("") {}
	TelnetAuthorizing &operator=(const TelnetAuthorizing &) { return *this; }

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

	/* member variables */
	enum TaState mState;
	Transfering *mpConn;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

