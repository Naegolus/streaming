
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "AdventCoding.h"

using namespace std;

uint32_t sizeFile = 0;
char line[10000];

int32_t numX = 0;
int32_t numY = 0;
uint32_t traceLenMax = 6000;

vector<string> hMap, traceMap, resMap;

uint32_t traceLenMin = 0;

uint32_t traceFind(
			vector<string> &tMap,
			int32_t posX = 0, int32_t posY = 0,
			uint32_t traceLen = 0, char last = 'a')
{
	if (last == 'z' and hMap[posY][posX] == 'E')
	{
		if (traceLenMin and traceLen >= traceLenMin)
			return 0 ;

		traceLenMin = traceLen;
		resMap = tMap;

		return 0 ;
	}

	if (tMap[posY][posX] != '.')
		return 0;

	++traceLen;

	if (traceLen > traceLenMax)
	{
		errLog(-1, "Reached max traceLen");
		return 0;
	}

	bool isStart = hMap[posY][posX] == 'S';
	char chNow = hMap[posY][posX];
	int32_t heightCurr = hMap[posY][posX];

	if (posY and (isStart or hMap[posY - 1][posX] - heightCurr < 2))
	{
		tMap[posY][posX] = '^';
		traceFind(tMap, posX, posY - 1, traceLen, chNow);
	}

	if (posY < numY - 1 and (isStart or hMap[posY + 1][posX] - heightCurr < 2))
	{
		tMap[posY][posX] = 'v';
		traceFind(tMap, posX, posY + 1, traceLen, chNow);
	}

	if (posX and (isStart or hMap[posY][posX - 1] - heightCurr < 2))
	{
		tMap[posY][posX] = '<';
		traceFind(tMap, posX - 1, posY, traceLen, chNow);
	}

	if (posX < numX - 1 and (isStart or hMap[posY][posX + 1] - heightCurr < 2))
	{
		tMap[posY][posX] = '>';
		traceFind(tMap, posX + 1, posY, traceLen, chNow);
	}

	tMap[posY][posX] = '.';

	return 0;
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

		if (!numX)
		{
			numX = strlen(line);
			numY = sizeFile / (numX + 1);
			procInfLog("Num X: %u", numX);
			procInfLog("Num Y: %u", numY);

			hMap.reserve(numY);
			traceMap.reserve(numY);
		}

		//procInfLog("Line: %s", line);
		hMap.emplace(hMap.end(), line);
		traceMap.emplace(traceMap.end(), string(numX, '.'));
	}

	fclose(pFile);
	pFile = NULL;

	traceFind(traceMap);
	uint32_t res1 = traceLenMin;

	for (vector<string>::iterator iter = resMap.begin(); iter != resMap.end(); ++iter)
		cout << *iter << endl;

	procInfLog("Result1: %u", res1);

	return Positive;
}

