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

#ifndef TRANSFERING_H
#define TRANSFERING_H

#include "Processing.h"

class Transfering : public Processing
{

public:

	virtual bool usable() { return false; }
	virtual ssize_t read(void *pBuf, size_t len) = 0;
	virtual void send(const void *pData, size_t len) = 0;

protected:

	Transfering(const char *name)
		: Processing(name)
	{}
	virtual ~Transfering() {}

private:

	Transfering() : Processing("") {}
	Transfering(const Transfering &) : Processing("") {}
	Transfering &operator=(const Transfering &) { return *this; }

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

