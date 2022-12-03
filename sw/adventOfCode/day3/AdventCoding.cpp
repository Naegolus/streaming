
#include <string>
#include <vector>
#include "AdventCoding.h"

using namespace std;

uint32_t prioSumGet(string commons)
{
	uint32_t u;
	uint32_t prioSum = 0;
	char ch = 0;

	for (u = 0; u < commons.size(); ++u)
	{
		ch = commons[u];

		if (ch <= 'z' and ch >= 'a')
			prioSum += ch - 'a' + 1;

		if (ch <= 'Z' and ch >= 'A')
			prioSum += ch - 'A' + 27;
	}

	return prioSum;
}

string commonsGet(const string &l, const string &r, bool pack = true)
{
	uint32_t u, v;
	string commonsAll;
	string commons;
	char last = 0;

	//infLog("%s - %s", l.c_str(), r.c_str());

	for (u = 0; u < l.size(); ++u)
	{
		for (v = 0; v < r.size(); ++v)
		{
			if (l[u] == r[v])
				commonsAll.push_back(l[u]);
		}
	}

	for (u = 0; u < commonsAll.size(); ++u)
	{
		if (commonsAll[u] == last)
			continue;

		commons.push_back(commonsAll[u]);
		last = commonsAll[u];
	}

	if (pack)
		return commons;

	return commonsAll;
}

void lineSplit(const char *pLine, string &l, string &r)
{
	uint32_t len = strlen(pLine);
	uint32_t len2 = len >> 1;
	l = string(pLine, len2);
	r = string(pLine + len2, len2);
}

string commonsGet(const char *pLine, bool pack = true)
{
	string l, r;
	lineSplit(pLine, l, r);
	return commonsGet(l, r, pack);
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	char line[128];
	string commons1, commons2, commons;
	uint32_t prioSum;
	uint32_t prioSumTotal = 0;
	vector<string> group;
	uint32_t groupCnt = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	group.clear();
	groupCnt = 0;

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		commons = commonsGet(line);
		//procInfLog("commons: %s", commons.c_str());

		prioSum = prioSumGet(commons);
		//prioSumTotal += prioSum;

		//procInfLog("prio   : %d", prioSum);

		group.push_back(line);
		++groupCnt;

		if (groupCnt >= 3)
		{
			procInfLog("g: %s", group[0].c_str());
			procInfLog("g: %s", group[1].c_str());
			procInfLog("g: %s", group[2].c_str());

			commons1 = commonsGet(group[0], group[1]);
			procInfLog("group commons 1: %s", commons1.c_str());

			commons2 = commonsGet(group[1], group[2]);
			procInfLog("group commons 2: %s", commons2.c_str());

			commons = commonsGet(commons1, commons2);
			procInfLog("group commons T: %s", commons.c_str());

			prioSum = prioSumGet(commons);
			prioSumTotal += prioSum;

			procInfLog("prio   : %d", prioSum);

			group.clear();
			groupCnt = 0;
		}
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("");
	procInfLog("Total: %d", prioSumTotal);

	return Positive;
}

