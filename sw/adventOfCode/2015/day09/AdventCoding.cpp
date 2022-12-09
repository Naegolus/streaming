
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
#if 0
	Value tmp;

	if (!dists.isMember("list"))
		dists["list"] = objectValue;

	tmp["src"] = src;
	tmp["dst"] = dst;

	dists["list"][to_string(dist)] = tmp;
#endif
}

bool nodeFound(list<string> &visited, const string &node)
{
	list<string>::const_iterator iter;

	for (iter = visited.begin(); iter != visited.end(); ++iter)
	{
		if (*iter == node)
			return true;
	}

	return false;
}

uint32_t calcShortest(
		list<string> &visited,
		uint8_t mode,
		uint32_t distTraveled = 0,
		uint32_t depth = 0)
{
	++depth;
	if (depth > 12)
	{
		errLog(-1, "Too deep");
		return 0;
	}

	uint32_t distNow;
	uint32_t dist, distRes = 0;

	Value::const_iterator iter;
	string nodeNext, nodeCurr;
	bool nextFound = false;

	//infLog("Depth %d", depth);

	for (iter = dists.begin(); iter != dists.end(); ++iter)
	{
		nodeNext = iter.key().asString();

		//infLog("Trying node %s", nodeNext.c_str());

		if (nodeFound(visited, nodeNext))
			continue;

		//infLog("Using node %s", nodeNext.c_str());

		nextFound = true;

		if (visited.size())
		{
			nodeCurr = visited.back();
			distNow = dists[nodeCurr][nodeNext].asUInt();
			//infLog("Traveling %d", distNow);
			distTraveled += distNow;
		}

		//infLog("Traveled  %d", distTraveled);

		visited.push_back(nodeNext);
		dist = calcShortest(visited, mode, distTraveled, depth);
		visited.pop_back();

		if (visited.size())
			distTraveled -= distNow;

		if (!distRes)
			distRes = dist;

		if (mode == 0)
		{
			if (dist < distRes)
				distRes = dist;
		}
		else
		{
			if (dist > distRes)
				distRes = dist;
		}
	}

	if (!nextFound)
		return distTraveled;

	//infLog("Depth %d: End", depth);

	return distRes;
}

uint32_t calcShortest(uint8_t mode = 0)
{
	uint32_t res;
	list<string> visited;

	res = calcShortest(visited, mode);

	return res;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");
	//FastWriter writer;
	StyledWriter writer;
	string str;
	uint32_t res1, res2;

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

	res1 = calcShortest();
	res2 = calcShortest(1);

	procInfLog("Result1: %d", res1);
	procInfLog("Result2: %d", res2);

	return Positive;
}

