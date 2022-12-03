
#include "AdventCoding.h"

using namespace std;

char line[10000];

/* member functions */
Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	int32_t floor = 0;
	uint32_t pos = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		for (size_t i = 0; i < strlen(line); ++i)
		{
			procInfLog("Instr: %c", line[i]);
			if (line[i] == '(')
				++floor;
			else
				--floor;

			if (!pos and floor == -1)
				pos = i + 1;
		}
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Result: %d", floor);
	procInfLog("Pos   : %d", pos);

	return Positive;
}

