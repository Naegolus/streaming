
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>
#include "AdventCoding.h"

using namespace std;
using namespace Json;

char line[10000];

string lookAndSay(string in, uint32_t cnt)
{
	if (!in.size())
		return "";

	uint32_t u, v;
	uint32_t cntLast;
	char chLast;
	string out;

	for (u = 0; u < cnt; ++u)
	{
		cntLast = 1;
		chLast = in[0];
		out = "";

		//infLog("Input at  %d/%d: %s", u, cnt, in.c_str());

		for (v = 1; v < in.size(); ++v)
		{
			if (in[v] == chLast)
			{
				++cntLast;
				continue;
			}

			out += to_string(cntLast);
			out += string(1, chLast);

			chLast = in[v];
			cntLast = 1;
		}

		out += to_string(cntLast);
		out += string(1, chLast);

		//infLog("Result at %d/%d: %s", u, cnt, out.c_str());
		//infLog("----");

		in = out;
	}

	return out;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");
	string res1, res2;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		procInfLog("Line: %s", line);

		res1 = lookAndSay(line, 40);
		res2 = lookAndSay(line, 50);
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Result1: %d", res1.size());
	procInfLog("Result2: %d", res2.size());

	return Positive;
}

