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

#ifndef AUTHORIZING_H
#define AUTHORIZING_H

#include "Processing.h"

class Authorizing : public Processing
{

public:

	std::string mGamerName;
	bool mSupporter;
	bool mAborted;

protected:

	Authorizing(const char *name)
		: Processing(name)
		, mGamerName("")
		, mSupporter(false)
		, mAborted(false)
	{}
	virtual ~Authorizing() {}

	uint32_t mKeyLastGotMs;

private:

	Authorizing() : Processing("") {}
	Authorizing(const Authorizing &) : Processing("") {}
	Authorizing &operator=(const Authorizing &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */

	/* member variables */

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

