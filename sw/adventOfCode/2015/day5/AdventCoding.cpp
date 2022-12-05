
#include <vector>
#include "AdventCoding.h"

using namespace std;

char line[10000];

/* member functions */
Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");
	size_t u, v;
	char chLast = 0;
	const char *vowels = "aeiou";
	uint8_t cntVowel;
	bool hasEnoughVowels, hasDoubleCh, hasForbidden, isNice, isNice2;
	bool hasDoubleDouble, hasSandwich;
	uint32_t cntIsNice = 0;
	uint32_t cntIsNice2 = 0;
	char sBuf[3];

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		chLast = 0;
		cntVowel = 0;
		hasEnoughVowels = false;
		hasDoubleCh = false;
		hasForbidden = false;
		isNice = false;
		isNice2 = false;

		hasDoubleDouble = false;
		hasSandwich = false;

		sBuf[0] = 0;
		sBuf[1] = 0;
		sBuf[2] = 0;

		for (u = 0; u < strlen(line); ++u)
		{
			for (v = 0; v < strlen(vowels); ++v)
			{
				if (line[u] == vowels[v])
					++cntVowel;
			}

			if (line[u] == chLast)
				hasDoubleCh = true;

			sBuf[0] = chLast;
			sBuf[1] = line[u];

			chLast = line[u];

			if (u >= 2 and line[u] == line[u - 2])
				hasSandwich = true;

			if (sBuf[0] and strstr(&line[u + 1], sBuf))
				hasDoubleDouble = true;
		}

		if (cntVowel >= 3)
			hasEnoughVowels = true;

		hasForbidden |= (strstr(line, "ab") != NULL);
		hasForbidden |= (strstr(line, "cd") != NULL);
		hasForbidden |= (strstr(line, "pq") != NULL);
		hasForbidden |= (strstr(line, "xy") != NULL);

		isNice = hasEnoughVowels and hasDoubleCh and not hasForbidden;
		isNice2 = hasDoubleDouble and hasSandwich;
#if 0
		procInfLog("Has enough:    %d", hasEnoughVowels);
		procInfLog("Has double:    %d", hasDoubleCh);
		procInfLog("Has forbidden: %d", hasForbidden);
		procInfLog("Is nice:       %d", isNice);
		procInfLog("");
#endif
		procInfLog("Line: %-20s > %d %d %d > %-10s | %d %d > %s",
						line,
						hasEnoughVowels,
						hasDoubleCh,
						hasForbidden,
						isNice ? "nice" : "naughty",
						hasDoubleDouble,
						hasSandwich,
						isNice2 ? "nice" : "naughty"
						);

		if (isNice)
			++cntIsNice;

		if (isNice2)
			++cntIsNice2;

	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Result1: %d", cntIsNice);
	procInfLog("Result2: %d", cntIsNice2);

	return Positive;
}

