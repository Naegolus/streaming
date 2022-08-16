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

#ifndef TIC_TAC_TOE_GAMING_H
#define TIC_TAC_TOE_GAMING_H

#include "Processing.h"
#include "TcpListening.h"

enum GamingPeerType {
	GamingPeerStd = 0,
};

struct GamingPeer
{
	enum GamingPeerType type;
	std::string typeDesc;
	std::string name;
	Processing *pProc;
};

class TicTacToeGaming : public Processing
{

public:

	static TicTacToeGaming *create()
	{
		return new (std::nothrow) TicTacToeGaming;
	}

protected:

	TicTacToeGaming();
	virtual ~TicTacToeGaming() {}

private:

	TicTacToeGaming(const TicTacToeGaming &) : Processing("") {}
	TicTacToeGaming &operator=(const TicTacToeGaming &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success initialize();
	Success process();
	void processInfo(char *pBuf, char *pBufEnd);

	void peerListUpdate();
	void peerRemove();
	void peerAdd(TcpListening *pListener, enum GamingPeerType peerType, const char *pTypeDesc);

	/* member variables */
	TcpListening *mpLst;
	std::list<struct GamingPeer> mPeerList;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

