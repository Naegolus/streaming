
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>
#include "AdventCoding.h"

using namespace std;
using namespace Json;

char line[10000];

string pwInc(const string &pwOld)
{
	string pwNew = pwOld;
	uint32_t idx = pwOld.size() - 1;

	while (1)
	{
		++pwNew[idx];

		if (pwNew[idx] <= 'z')
			break;

		pwNew[idx] = 'a';
		--idx;
	}

	return pwNew;
}

bool pwOk(const string &pwNew)
{
	if (pwNew.size() != 8)
		return false;

	uint32_t u;
	char ch, chLast = 0, chPair1 = 0;
	bool hasStraight = false;
	bool hasPair = false;
	bool hasPairDouble = false;

	for (u = 0; u < pwNew.size(); ++u)
	{
		ch = pwNew[u];
		if (u)
			chLast = pwNew[u - 1];

		if (ch == 'i' or ch == 'o' or ch == 'l')
			return false;

		if (u >= 2)
		{
			if (pwNew[u - 2] == pwNew[u - 1] - 1 and
				pwNew[u - 1] == pwNew[u - 0] - 1)
				hasStraight = true;
		}

		if (!u or hasPairDouble or ch != chLast or ch == chPair1)
			continue;

		if (hasPair)
			hasPairDouble = true;

		hasPair = true;
		chPair1 = ch;
	}
#if 0
	infLog("Line: %-20s > %d %d %d",
					pwNew.c_str(),
					hasPair,
					hasPairDouble,
					hasStraight
					);
#endif
	if (!hasStraight)
		return false;

	if (!hasPairDouble)
		return false;

	return true;
}

string pwNewCalc(const string &pwOld)
{
	string pwNew = pwOld;

	do
	{
		pwNew = pwInc(pwNew);
	} while (!pwOk(pwNew));

	return pwNew;
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

		res1 = pwNewCalc(line);
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Result1: %s", res1.c_str());
	res2 = pwNewCalc(res1);
	procInfLog("Result2: %s", res2.c_str());

	return Positive;
}

