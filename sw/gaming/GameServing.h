/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 17.08.2022

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

#ifndef GAME_SERVING_H
#define GAME_SERVING_H

#include "Processing.h"

class GameServing : public Processing
{

public:

	static GameServing *create()
	{
		return new (std::nothrow) GameServing;
	}

protected:

	GameServing();
	virtual ~GameServing() {}

private:

	GameServing(const GameServing &) : Processing("") {}
	GameServing &operator=(const GameServing &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success initialize();
	Success process();
	Success shutdown();
	void processInfo(char *pBuf, char *pBufEnd);

	/* member variables */

	/* static functions */

	/* static variables */

	/* constants */

};

#endif
