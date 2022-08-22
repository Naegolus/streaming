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

#ifndef GAMING_H
#define GAMING_H

#include <list>
#include <mutex>

#include "Processing.h"

class Gaming;

struct TypeListElem
{
	std::string name;
	std::string art;
	std::string author;
	std::string authorPage;
	std::string desc;
	Gaming *(*pFctCreate)();
};

class Gaming : public Processing
{

public:

	std::string mGameName;
	std::string mType;
	std::string mFlags;

	static std::mutex mtxGamesList;
	static std::list<Gaming *> gamesList;
	static std::list<struct TypeListElem> typesList;

protected:

	Gaming(const char *name)
		: Processing(name)
		, mGameName("")
		, mType("")
		, mFlags("")
	{}
	virtual ~Gaming() {}

private:

	Gaming() : Processing("") {}
	Gaming(const Gaming &) : Processing("") {}
	Gaming &operator=(const Gaming &) { return *this; }

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	Success initialize();
	Success process();
	Success shutdown();
	void processInfo(char *pBuf, char *pBufEnd);

	virtual Success gameProcess() = 0;

	/* member variables */

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

