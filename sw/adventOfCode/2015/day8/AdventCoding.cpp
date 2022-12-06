
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "AdventCoding.h"

using namespace std;

char line[10000];

void lineParse(const char *pLine,
				uint8_t *numCode,
				uint8_t *numData,
				char *pBuf,
				char *pBufEnd)
{
	uint32_t u;
	uint8_t state = 0;
	uint8_t ch;

	*numCode = 0;
	*numData = 0;

	pBuf[0] = 0;
	dInfo("\"");

	for (u = 0; u < strlen(pLine); ++u)
	{
		ch = pLine[u];

		switch (state)
		{
		case 0: /* Data */

			if (ch == '\\')
			{
				dInfo("\\\\");
				++*numCode;
				state = 1;
				break;
			}

			if (ch == '"')
			{
				dInfo("\\\"");
				++*numCode;
				break;
			}

			dInfo("%c", ch);
			++*numData;

			break;
		case 1: /* First escape */

			if (ch == 'x')
			{
				dInfo("x%c%c", line[u + 1], line[u + 2]);
				*numCode += 2;
				u += 2;
			} else
			if (ch == '"')
				dInfo("\\\"");
			else
			if (ch == '\\')
				dInfo("\\\\");

			++*numData;
			state = 0;

			break;
		default:
			break;
		}
	}

	dInfo("\"");
	*numCode += *numData;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");
	uint8_t numCode1, numData1;
	uint8_t numCode2, numData2;
	uint32_t diffSum1 = 0;
	uint32_t diffSum2 = 0;
	char bufEnc[1024];
	char bufEnc2[1024];
	char *pBufEnd = bufEnc + sizeof(bufEnc);

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		procInfLog("Line: %s", line);

		pBufEnd = bufEnc + sizeof(bufEnc);
		lineParse(line, &numCode1, &numData1, bufEnc, pBufEnd);
		infLog("%s", bufEnc);

		pBufEnd = bufEnc2 + sizeof(bufEnc2);
		lineParse(bufEnc, &numCode2, &numData2, bufEnc2, pBufEnd);

		//procInfLog("Code: %d", numCode);
		//procInfLog("Data: %d", numData);

		diffSum1 += (numCode1 - numData1);
		diffSum2 += (numCode2 - numCode1);
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("Result1: %d", diffSum1);
	procInfLog("Result2: %d", diffSum2);

	return Positive;
}

