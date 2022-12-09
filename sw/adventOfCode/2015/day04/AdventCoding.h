/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 03.12.2022

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

#ifndef ADVENT_CODING_H
#define ADVENT_CODING_H

#include "Processing.h"

class AdventCoding : public Processing
{

public:

	static AdventCoding *create()
	{
		return new (std::nothrow) AdventCoding;
	}

protected:

	AdventCoding()	: Processing("AdventCoding") {}
	virtual ~AdventCoding() {}

private:

	AdventCoding(const AdventCoding &) : Processing("") {}
	AdventCoding &operator=(const AdventCoding &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success initialize();
	Success process();

	/* member variables */

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

