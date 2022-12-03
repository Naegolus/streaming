
#include <vector>
#include "AdventCoding.h"

using namespace std;

struct GameStrategy
{
	char chIn;
	char chCrypt;
	uint32_t chOut;
	uint32_t score;
	uint32_t chOutNew;
	uint32_t scoreNew;
};

// A Rock
// B Paper
// C Scissors

// X Rock		1
// Y Paper	2
// Z Scissors	3

// --- New
// Lose		0
// Draw		3
// Win		6

// X Lose		0
// Y Draw		3
// Z Win		6

GameStrategy gs[] =
{
	{'A', 'X', 'A', 1 + 3, 'C', 0 + 3},
	{'A', 'Y', 'B', 2 + 6, 'A', 3 + 1},
	{'A', 'Z', 'C', 3 + 0, 'B', 6 + 2},
	{'B', 'X', 'A', 1 + 0, 'A', 0 + 1},
	{'B', 'Y', 'B', 2 + 3, 'B', 3 + 2},
	{'B', 'Z', 'C', 3 + 6, 'C', 6 + 3},
	{'C', 'X', 'A', 1 + 6, 'B', 0 + 2},
	{'C', 'Y', 'B', 2 + 0, 'C', 3 + 3},
	{'C', 'Z', 'C', 3 + 3, 'A', 6 + 1},
};

uint32_t scoreGet(char in, char crypt, bool takeNew = false)
{
	for (size_t i = 0; i < sizeof(gs) / sizeof(gs[0]); ++i)
	{
		if (in == gs[i].chIn and crypt == gs[i].chCrypt)
		{
			if (takeNew)
				return gs[i].scoreNew;
			else
				return gs[i].score;
		}
	}

	return 0;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	char line[128];
	char in, crypt;
	uint32_t score = 0;
	uint32_t scoreSum = 0;
	uint32_t scoreSumNew = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		//procInfLog("Line: %s", line);

		in = line[0];
		crypt = line[2];

		score = scoreGet(in, crypt);
		scoreSum += score;

		score = scoreGet(in, crypt, true);
		scoreSumNew += score;

		procInfLog("%c - %c: score = %d", in, crypt, score);
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("");
	procInfLog("Sum  : %d", scoreSum);
	procInfLog("New  : %d", scoreSumNew);
	procInfLog("");

	return Positive;
}

