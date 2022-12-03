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

#include <vector>
#include "AdventCoding.h"

using namespace std;

#define LOG_LVL	0

AdventCoding::AdventCoding()
	: Processing("AdventCoding")
{}

/* member functions */
Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	char line[128];
	vector<uint32_t> deers;
	uint32_t calCurr = 0;
	uint32_t calSum = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	deers.clear();

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[0] == '\n')
		{
			//procInfLog("Finishing deer: %d", calCurr);

			deers.push_back(calCurr);
			calCurr = 0;

			continue;
		}

		calCurr += atoi(line);
	}

	sort(deers.begin(), deers.end());
	reverse(deers.begin(), deers.end());

	for (size_t i = 0; i < 3; ++i)
	{
		procInfLog("Deer %d: %d", i + 1, deers[i]);
		calSum += deers[i];
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Sum cal: %d", calSum);

	return Positive;
}

void AdventCoding::processInfo(char *pBuf, char *pBufEnd)
{
	(void)pBuf;
	(void)pBufEnd;
}

/* static functions */

