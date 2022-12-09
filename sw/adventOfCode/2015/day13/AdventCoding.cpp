
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>
#include "AdventCoding.h"

using namespace std;
using namespace Json;

char line[10000];

Value ppl;
list<list<string> > circles;

void lineParse(const char *pLine)
{
	char src[32];
	char dst[32];
	char what[8];
	int32_t happ;
	int res;

	res = sscanf(pLine, "%s would %s %d happiness units by sitting next to %s", src, what, &happ, dst);
	if (res != 4)
	{
		errLog(-1, "Could not parse line");
		return;
	}

	dst[strlen(dst) - 1] = 0;

	if (!strcmp(what, "lose"))
		happ = -happ;

	if (!ppl.isMember(src))
		ppl[src] = objectValue;
	ppl[src][dst] = happ;
}

bool personFound(list<string> &circle, const string &name)
{
	list<string>::const_iterator iter;

	for (iter = circle.begin(); iter != circle.end(); ++iter)
	{
		if (*iter == name)
			return true;
	}

	return false;
}

void circleFind(list<string> &circle, uint32_t depth = 0)
{
	++depth;
	if (depth > 12)
	{
		errLog(-1, "Too deep");
		return;
	}

	Value::const_iterator person;
	string nameNext;
	bool nextFound = false;

	//infLog("--- Depth %d", depth);

	for (person = ppl.begin(); person != ppl.end(); ++person)
	{
		nameNext = person.key().asString();

		//infLog("Trying person %s", nameNext.c_str());

		if (personFound(circle, nameNext))
			continue;

		//infLog("Using person %s", nameNext.c_str());

		nextFound = true;

		circle.push_back(nameNext);
		circleFind(circle, depth);
		circle.pop_back();
	}

	if (!nextFound)
	{
#if 0
		list<string>::iterator name;

		for (name = circle.begin(); name != circle.end(); ++name)
		{
			if (name != circle.begin())
				cout << " -> ";
			cout << *name;
		}
		cout << endl;
#endif
		circles.push_back(circle);
	}

	//infLog("--- Depth %d: End", depth);
}

void circlesFind()
{
	list<string> circle;

	circles.clear();
	circleFind(circle);
}

int32_t happinessHighest()
{
	list<list<string> >::iterator circle;
	list<string>::iterator name, nameLast, nameNext;
	string nameLeft, nameRight;
	int32_t happ, happRes;
	bool first = true;
	uint32_t u;

	for (circle = circles.begin(); circle != circles.end(); ++circle)
	{
		if (circle->size() < 2)
			continue;

		happ = 0;
		u = 0;
		nameNext = circle->begin();
		++nameNext;

		for (name = circle->begin(); name != circle->end(); ++name, ++nameNext, ++u)
		{
			if (u)
				nameLeft = *nameLast;
			else
				nameLeft = circle->back();

			if (u < circle->size() - 1)
				nameRight = *nameNext;
			else
				nameRight = circle->front();

			happ += ppl[*name][nameLeft].asInt() + ppl[*name][nameRight].asInt();

			nameLast = name;
		}

		if (first)
		{
			happRes = happ;
			first = false;
		}

		if (happ > happRes)
			happRes = happ;
	}

	return happRes;
}

void myselfAdd()
{
	Value::iterator person;
	string name;

	ppl["dude"] = objectValue;

	for (person = ppl.begin(); person != ppl.end(); ++person)
	{
		name = person.key().asString();

		if (name == "dude")
			continue;

		(*person)["dude"] = 0;
		ppl["dude"][name] = 0;
	}
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");
	//FastWriter writer;
	StyledWriter writer;
	string str;
	int32_t happ;

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
#if 0
	str = writer.write(ppl);
	cout << str;
#endif
	circlesFind();

	happ = happinessHighest();
	procInfLog("Result1: %d", happ);

	myselfAdd();
#if 0
	str = writer.write(ppl);
	cout << str;
#endif
	circlesFind();

	happ = happinessHighest();
	procInfLog("Result2: %d", happ);

	return Positive;
}

