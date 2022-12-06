
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>
#include "AdventCoding.h"

using namespace std;
using namespace Json;

char line[10000];

Value dists;

void lineParse(const char *pLine)
{
	char src[32];
	char dst[32];
	uint32_t dist;
	int res;

	res = sscanf(pLine, "%s to %s = %u", src, dst, &dist);
	if (res != 3)
	{
		errLog(-1, "Could not parse line");
		return;
	}

	if (!dists.isMember(src))
		dists[src] = objectValue;
	dists[src][dst] = dist;

	if (!dists.isMember(dst))
		dists[dst] = objectValue;
	dists[dst][src] = dist;
}

uint32_t calc()
{
	return 0;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	//FastWriter writer;
	StyledWriter writer;
	string str;
	uint32_t res1;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		procInfLog("Line: %s", line);

		lineParse(line);
	}

	fclose(pFile);
	pFile = NULL;

	str = writer.write(dists);
	cout << str;

	res1 = calc();

	procInfLog("Result1: %d", res1);

	return Positive;
}

