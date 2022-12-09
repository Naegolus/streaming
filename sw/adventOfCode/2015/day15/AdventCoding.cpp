
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>
#include "AdventCoding.h"

using namespace std;
using namespace Json;

char line[10000];

Value ingredients;

void lineParse(const char *pLine)
{
	char name[32];
	int32_t cap, dur, flav, text, cal;
	int res;

	res = sscanf(pLine, "%s capacity %d, durability %d, flavor %d, texture %d, calories %d",
						name, &cap, &dur, &flav, &text, &cal);
	if (res != 6)
	{
		errLog(-1, "Could not parse line");
		return;
	}

	name[strlen(name) - 1] = 0;

	if (!ingredients.isMember(name))
		ingredients[name] = objectValue;

	ingredients[name]["capacity"] = cap;
	ingredients[name]["durability"] = dur;
	ingredients[name]["flavor"] = flav;
	ingredients[name]["texture"] = text;
	ingredients[name]["calories"] = cal;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	//FastWriter writer;
	StyledWriter writer;
	string str;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		lineParse(line);
	}

	fclose(pFile);
	pFile = NULL;
#if 1
	str = writer.write(ingredients);
	cout << str;
#endif
	procInfLog("Result1: ");

	return Positive;
}

