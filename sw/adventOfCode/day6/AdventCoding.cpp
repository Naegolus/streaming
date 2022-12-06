
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "AdventCoding.h"

using namespace std;

char line[10000];

bool prefixDifferent(const char *pCurrent, uint32_t cntDiff = 4)
{
	const char *pStart;
	const char *pCmp;

	for (pStart = pCurrent - cntDiff; pStart < pCurrent; ++pStart)
	{
		//infLog("Start: %c", *pStart);
		for (pCmp = pStart + 1; pCmp < pCurrent; ++pCmp)
		{
			//infLog("  %c", *pCmp);

			if (*pStart == *pCmp)
				return false;
		}
	}

	return true;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	uint32_t u;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		for (u = 4; u < strlen(line) + 1; ++u)
		{
			if (!prefixDifferent(&line[u], 14))
				continue;

			line[u] = '_';

			procInfLog("Line: %-40s > %u", line, u);

			break;
		}
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Result: %d", u);

	return Positive;
}

