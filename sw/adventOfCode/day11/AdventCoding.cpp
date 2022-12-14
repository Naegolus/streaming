
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "AdventCoding.h"
#include "AocMonkey.h"

using namespace std;
using namespace Json;

uint32_t sizeFile = 0;
char line[10000];

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

	StyledWriter writer;
	//cout << writer.write(notes);

	//monkeyItemsPrint();
	//cout << "-------" << endl;

	superModCalc();

	numRoundsExec(10000, false);
	//cout << writer.write(notes);

	//monkeyItemsPrint();
	//cout << "-------" << endl;
	uint64_t res1 = monkeyCntThrownPrint();

	procInfLog("Result1: %lu", res1);

	return Positive;
}

