
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "AdventCoding.h"

using namespace std;

char line[10000];

const uint32_t lenSide = 1000;
uint32_t lights[lenSide][lenSide];

void cmdExec(char *pStart)
{
	//const char *pCmd = pStart;
	int res;
	uint32_t cmd = 0;
	uint32_t x1, y1, x2, y2;
	uint32_t x, y;

	if (strstr(pStart, "turn off"))
	{
		pStart += strlen("turn off");
		cmd = 1;
	}

	if (strstr(pStart, "turn on"))
	{
		pStart += strlen("turn on");
		cmd = 2;
	}

	if (strstr(pStart, "toggle"))
	{
		pStart += strlen("toggle");
		cmd = 3;
	}

	*pStart = 0;
	++pStart;

	res = sscanf(pStart, "%u,%u through %u,%u", 
				&x1, &y1,
				&x2, &y2);
	if (res != 4)
	{
		errLog(-1, "Could not parse command");
		return;
	}
#if 0
	infLog("Cmd: %s (%d)", pCmd, cmd);
	infLog("Coord: %u,%u - %u,%u", x1, y1, x2, y2);
#endif
	for (y = y1; y <= y2; ++y)
	{
		for (x = x1; x <= x2; ++x)
		{
			if (cmd == 1)
			{
				if (lights[y][x])
					--lights[y][x];
			}
			else
			if (cmd == 2)
				++lights[y][x];
			else
				lights[y][x] += 2;
		}
	}
}

uint32_t lightsCount()
{
	uint32_t x, y;
	uint32_t cnt = 0;

	for (y = 0; y < lenSide; ++y)
	{
		for (x = 0; x < lenSide; ++x)
		{
			if (lights[y][x])
				cnt += lights[y][x];
		}
	}

	return cnt;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	uint32_t cnt = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	memset(lights, 0, sizeof(lights));

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		cmdExec(line);
	}

	cnt = lightsCount();

	fclose(pFile);
	pFile = NULL;

	procInfLog("Result: %u", cnt);

	return Positive;
}

