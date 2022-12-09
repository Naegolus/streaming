
#include <string>
#include <vector>
#include "AdventCoding.h"

using namespace std;

char line[10000];

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	int res;
	uint32_t min1, max1, min2, max2;
	uint32_t overlapSum = 0;
	uint32_t completeOverlapSum = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		res = sscanf(line, "%u-%u,%u-%u", &min1, &max1, &min2, &max2);
		if (res != 4)
			return procErrLog(-1, "Could not parse line");

		procInfLog("%d - %d | %d - %d", min1, max1, min2, max2);

		if ((min1 >= min2 and max1 <= max2) or (min2 >= min1 and max2 <= max1))
			++completeOverlapSum;

		if ((max2 >= min1 and min2 <= max1) or (max1 >= min2 and min1 <= max2))
			++overlapSum;
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("");
	procInfLog("Result1: %d", completeOverlapSum);
	procInfLog("Result2: %d", overlapSum);

	return Positive;
}

