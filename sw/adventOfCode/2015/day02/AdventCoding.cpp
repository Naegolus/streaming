
#include "AdventCoding.h"

using namespace std;

char line[10000];

/* member functions */
Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	uint32_t cnt = 0;
	int32_t l, w, h;
	uint32_t a1, a2, a3;
	uint32_t amin, a, asum = 0;
	uint32_t umin1, umin2, u, usum = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		cnt = sscanf(line, "%dx%dx%d", &l, &w, &h);
		if (cnt != 3)
			return procErrLog(-1, "Could not parse line");

		procInfLog("%d - %d - %d", l, w, h);

		a1 = l * w;
		a2 = w * h;
		a3 = h * l;

		amin = MIN(a1, a2);
		amin = MIN(amin, a3);

		a = 2 * a1 + 2 * a2 + 2 * a3 + amin;
		asum += a;
#if 0
		procInfLog("a1: %d", a1);
		procInfLog("a2: %d", a2);
		procInfLog("a3: %d", a3);
		procInfLog("amin: %d", amin);
		procInfLog("a: %d", a);
		procInfLog("");
#endif
		umin1 = MIN(l, w);
		umin2 = MIN(MAX(l, w), h);

		u = 2 * umin1 + 2 * umin2 + l * w * h;
		usum += u;

		procInfLog("umin1: %d", umin1);
		procInfLog("umin2: %d", umin2);
	}

	fclose(pFile);
	pFile = NULL;

	procInfLog("asum: %d", asum);
	procInfLog("usum: %d", usum);

	return Positive;
}

