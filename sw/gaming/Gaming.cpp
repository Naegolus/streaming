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

#include "Gaming.h"

using namespace std;

#define LOG_LVL	0

mutex Gaming::mtxGamesList;
list<Gaming *> Gaming::gamesList;
vector<struct TypeListElem> Gaming::typesList;

/* member functions */
Success Gaming::initialize()
{
	return Positive;
}

Success Gaming::process()
{
	try
	{
		return gameProcess();
	}
	catch (...)
	{
		return procErrLog(-1, "Unhandled gaming exception");
	}
}

Success Gaming::shutdown()
{
	return Positive;
}

void Gaming::processInfo(char *pBuf, char *pBufEnd)
{
	(void)pBuf;
	(void)pBufEnd;
}

/* static functions */

void Gaming::gameRegister(const std::string &name,
			Gaming *(*pFctCreate)(),
			void (*pFctInfoSet)(struct TypeListElem &type))
{
	struct TypeListElem type;

	type.name = name;
	type.pFctCreate = pFctCreate;

	pFctInfoSet(type);

	Gaming::typesList.push_back(type);
}

Gaming *Gaming::create(const string &type)
{
	vector<struct TypeListElem>::iterator iter;

	iter = typesList.begin();
	for (; iter != typesList.end(); ++iter)
	{
		if (iter->name != type)
			continue;

		return iter->pFctCreate();
	}

	return NULL;
}

