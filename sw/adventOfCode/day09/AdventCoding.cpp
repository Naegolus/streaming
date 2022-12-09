
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "AdventCoding.h"

using namespace std;

struct Pos
{
	int32_t x;
	int32_t y;
};

uint32_t sizeFile = 0;
char line[10000];

vector<Pos> rope(10, Pos());

vector<string> positions;

void lineParse(const char *pLine)
{
	int res;
	char dir;
	uint32_t steps;

	res = sscanf(pLine, "%c %u", &dir, &steps);
	if (res != 2)
	{
		errLog(-1, "Could not parse line");
		return;
	}

	//infLog("####################");
	//infLog("%c - %u", dir, steps);

	uint32_t u, v;
	int32_t diffX, diffY;
	uint32_t aDiffX, aDiffY;
	string posNew;

	for (u = 0; u < steps; ++u)
	{
		diffX = 0;
		diffY = 0;

		if (dir == 'R')
			diffX = 1;
		else
		if (dir == 'L')
			diffX = -1;
		else
		if (dir == 'U')
			diffY = 1;
		else
			diffY = -1;

		//infLog("####################");
		rope[0].x += diffX;
		rope[0].y += diffY;
		//infLog("n%d: %d / %d", 0, rope[0].x, rope[0].y);

		for (v = 0; v < rope.size() - 1; ++v)
		{
			//infLog("------");
			//infLog("n%d: %d / %d", v, rope[v].x, rope[v].y);

			diffX = rope[v].x - rope[v + 1].x;
			diffY = rope[v].y - rope[v + 1].y;

			aDiffX = abs(diffX);
			aDiffY = abs(diffY);

			if (aDiffY == 2 and aDiffX == 2)
			{
				rope[v + 1].y += diffY >> 1;
				rope[v + 1].x += diffX >> 1;
			}
			else
			if (aDiffX == 2)
			{
				rope[v + 1].x += diffX >> 1;
				rope[v + 1].y += diffY;
			}
			else
			if (aDiffY == 2)
			{
				rope[v + 1].x += diffX;
				rope[v + 1].y += diffY >> 1;
			}

			//infLog("n%d: %d / %d", v + 1, rope[v + 1].x, rope[v + 1].y);

			if (v + 1 < rope.size() - 1)
				continue;

			posNew = to_string(rope[v + 1].x) + " / " + to_string(rope[v + 1].y);
			//infLog("Item %d: %s", v + 1, posNew.c_str());

			positions.push_back(posNew);
		}
	}
}

void posPrint()
{
	vector<string>::iterator iter;

	for (iter = positions.begin(); iter != positions.end(); ++iter)
		cout << *iter << endl;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	fseek(pFile, 0, SEEK_END);
	sizeFile = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	procInfLog("File size: %d", sizeFile);

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		lineParse(line);
	}

	fclose(pFile);
	pFile = NULL;

	//posPrint();

	sort(positions.begin(), positions.end());
	procInfLog("Result1: %d", positions.size());
	//posPrint();

	positions.erase(unique(positions.begin(), positions.end()), positions.end());
	procInfLog("Result1: %d", positions.size());
	//posPrint();

	return Positive;
}

