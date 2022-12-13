
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "AdventCoding.h"

using namespace std;

uint32_t sizeFile = 0;
char line[10000];

uint32_t clockCycle = 0;
uint32_t cntCheck = 0;
int32_t regX = 1;
int32_t sig = 0;
int32_t sigSum = 0;

int32_t posCrt = 0;
#define dPosCrtMax	40

void cycleInc()
{
	bool found = false;
	int32_t posSprite = 0;
	++clockCycle;

	// print
	for (int8_t u = -1; u < 2; ++u)
	{
		posSprite = regX + u;

		if (posSprite < 0 or posSprite >= dPosCrtMax)
			continue;

		if (posCrt == posSprite)
		{
			found = true;
			break;
		}
	}

	if (found)
		cout << "#";
	else
		cout << ".";

	// increment crt
	++posCrt;
	if (posCrt >= dPosCrtMax)
	{
		cout << endl;
		posCrt = 0;
	}
}

void sigCheck()
{
	if (clockCycle == 20 + cntCheck * 40)
	{
		sig = clockCycle * regX;
		sigSum += sig;
		++cntCheck;
		//wrnLog("Sig: %d = %u * %d", sig, clockCycle, regX);
	}
}

void lineParse(const char *pLine)
{
	int res;
	char bufInstr[16];
	int32_t val;

	if (!strncmp(pLine, "noop", strlen("noop")))
	{
		cycleInc();
		sigCheck();
		return;
	}

	res = sscanf(pLine, "%s %d", bufInstr, &val);
	if (res == 2)
	{
		for (uint8_t u = 0; u < 2; ++u)
		{
			cycleInc();
			sigCheck();
		}

		//infLog("-------------");
		//infLog("%s", pLine);
		//infLog("cyc %d, regX %d", clockCycle, regX);
		regX += val;
		//infLog("cyc %d, regX %d", clockCycle, regX);

		return;
	}
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

	procInfLog("Result1: %d", sigSum);

	return Positive;
}

