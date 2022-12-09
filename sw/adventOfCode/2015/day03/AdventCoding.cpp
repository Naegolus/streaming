
#include <vector>
#include "AdventCoding.h"

using namespace std;

char line[10000];

/* member functions */
Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	uint32_t x1, y1, x2, y2;
	uint32_t *pX = &x1, *pY = &y1;
	size_t i;
	char d;
	string strCoord;
	vector<string> coords;
	uint32_t houses, housesSum = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		procInfLog("Line: %s", line);

		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
		pX = &x1;
		pY = &y1;

		coords.clear();

		strCoord = to_string(*pX) + " - " + to_string(*pY);
		//procInfLog("%s", strCoord.c_str());
		coords.push_back(strCoord);

		for (i = 0; i < strlen(line); ++i)
		{
			d = line[i];

			if (d == '>')
				++*pX;
			else
			if (d == '<')
				--*pX;
			else
			if (d == '^')
				++*pY;
			else
				--*pY;

			//procInfLog("Dir: %c", d);

			strCoord = to_string(*pX) + " - " + to_string(*pY);
			//procInfLog("%s", strCoord.c_str());
			coords.push_back(strCoord);

			if (pX == &x1)
			{
				pX = &x2;
				pY = &y2;
			}
			else
			{
				pX = &x1;
				pY = &y1;
			}

		}

		sort(coords.begin(), coords.end());
		coords.erase(unique(coords.begin(), coords.end()), coords.end());

		for (vector<string>::iterator iter = coords.begin(); iter != coords.end(); ++iter)
			procInfLog("coord: %s", iter->c_str());
		procInfLog("");

		houses = coords.size();
		housesSum += houses;
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Num houses: %d", housesSum);

	return Positive;
}

