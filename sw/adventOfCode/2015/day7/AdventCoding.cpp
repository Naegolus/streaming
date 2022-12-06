
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "AdventCoding.h"
#include "Circuit.h"

using namespace std;

char line[10000];

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	uint16_t sig;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		assignmentAdd(line);
		notAdd(line);
		gateAdd(line);
	}

	fclose(pFile);
	pFile = NULL;
#if 0
	sig = wireSig("a");
#endif
	sig = 46065;
	procInfLog("Result1: %u", sig);

	wireAssign("b", sig);

	sig = wireSig("a");
	procInfLog("Result2: %u", sig);

	return Positive;
}

