
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "AdventCoding.h"

using namespace std;

char line[10000];

vector<list<char> > crates;

void listPrint(list<char> &lst)
{
	for (list<char>::iterator v = lst.begin(); v != lst.end(); ++v)
		cout << "  [" << *v << "], ";
	cout << endl;
}

void cratesPrint()
{
	infLog("-------------------");
	infLog("Printing crates");

	for (size_t i = 0; i < crates.size(); ++i)
		listPrint(crates[i]);

	infLog("Printing crates: done");
}

void stateZeroGet(FILE *pFile)
{
	size_t lineLen = 0;
	size_t numElements = 0;
	size_t pos, len = 0;
	size_t i = 0;

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		if (line[0] == 'm')
			break;

		len = strlen(line);
		if (!len)
		{
			if (!lineLen)
				continue;
			else
				break;
		}

		if (!lineLen)
		{
			lineLen = len - 3;
			numElements = (lineLen >> 2) + 1;
			infLog("numElements = %d", numElements);

			crates.resize(numElements);
		}

		if (len < lineLen)
			continue;

		if (line[1] == '1')
			continue;

		//infLog("Line: %s", line);
		//infLog("len : %d", len);

		for (i = 0; i < numElements; ++i)
		{
			pos = 1 + i * 4;
			if (line[pos] == ' ')
				continue;

			//infLog("char : %c", line[pos]);

			crates[i].push_back(line[pos]);
		}
	}

	infLog("Zero state parsing done");
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");
	int res;
	uint32_t num, idxSrc, idxDst;
	size_t i = 0;
	char crate;
	string frontCrates = "";
	list<char> lstTmp;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	stateZeroGet(pFile);
	cratesPrint();

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		res = sscanf(line, "move %u from %u to %u", &num, &idxSrc, &idxDst);
		if (res != 3)
			return procErrLog(-1, "Could not parse line");

		//procInfLog("Line: %s", line);

		--idxSrc;
		--idxDst;

		for (i = 0; i < num; ++i)
		{
			if (!crates[idxSrc].size())
				break;

			crate = crates[idxSrc].front();
			//infLog("Got crate [%c] from %d", crate, idxSrc);
			//infLog("Removing crate from %d", idxSrc);
			crates[idxSrc].pop_front();

			//infLog("Pushing crate to tmp");
			//crates[idxDst].push_front(crate);
			lstTmp.push_back(crate);

			//infLog("Pushing done");
		}

		//listPrint(lstTmp);

		while (lstTmp.size())
		{
			crate = lstTmp.back();
			lstTmp.pop_back();
			//infLog("Pushing crate [%c] to %d", crate, idxDst);
			crates[idxDst].push_front(crate);
		}

		//cratesPrint();
	}

	cratesPrint();

	for (i = 0; i < crates.size(); ++i)
	{
		if (!crates[i].size())
			continue;

		crate = crates[i].front();
		//infLog("Got crate [%c] from %d", crate, i);

		frontCrates.push_back(crate);
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("");
	procInfLog("Result1: %s", frontCrates.c_str());

	return Positive;
}

