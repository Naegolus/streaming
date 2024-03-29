/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 17.08.2022

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

#ifndef GAMING_H
#define GAMING_H

#include <list>
#include <vector>
#include <mutex>
#include <jsoncpp/json/json.h>

#include "Processing.h"
#include "Pipe.h"

class Gaming;

struct TypeListElem
{
	// Set by Gaming()
	std::string name;
	Gaming *(*pFctCreate)();

	// Set by <Specific>Gaming()
	std::string art;
	std::string author;
	std::string authorPage;
	std::string desc;
};

class Gaming : public Processing
{

public:

	static void gameRegister(const std::string &name,
				Gaming *(*pFctCreate)(),
				void (*pFctInfoSet)(struct TypeListElem &type));

	static Gaming *create(const std::string &type);

	std::string mGameName;
	std::string mType;
	std::string mFlags;
	Json::Value mGameState;

	static std::mutex mtxGamesList;
	static std::list<Gaming *> gamesList;
	static std::vector<struct TypeListElem> typesList;

	Pipe<Json::Value> in;
	Pipe<Json::Value> out;

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

	/* member variables */

	/* static functions */

	/* static variables */

	/* constants */

};

#define dGameRegister(g) Gaming::gameRegister(#g, g ## Gaming::create, g ## Gaming::gameInfoSet)

#endif

