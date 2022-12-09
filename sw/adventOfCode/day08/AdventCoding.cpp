
#include <iostream>
#include <string>
#include <vector>
#include "AdventCoding.h"
#include "AocTree.h"

using namespace std;

char line[10000];

uint32_t cntVisible = 0;
uint32_t scenicMax = 0;

void treeCalc()
{
	uint32_t r, c;
	uint32_t scenic;

	cntVisible += 2 * (cntRows + cntCols - 2);

	for (r = 1; r < cntRows - 1; ++r)
	{
		for (c = 1; c < cntCols - 1; ++c)
			cntVisible += treeVisible(r, c);
	}

	for (r = 0; r < cntRows; ++r)
	{
		for (c = 0; c < cntCols; ++c)
		{
			scenic = treeScenic(r, c);

			if (scenic > scenicMax)
				scenicMax = scenic;
		}
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

	//treesPrint();

	treeCalc();

	procInfLog("Result1: %d", cntVisible);
	procInfLog("Result2: %d", scenicMax);

	return Positive;
}

