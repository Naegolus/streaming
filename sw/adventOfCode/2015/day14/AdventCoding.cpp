
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>
#include "AdventCoding.h"

using namespace std;
using namespace Json;

char line[10000];

Value reindeers;

void lineParse(const char *pLine)
{
	char rd[32];
	uint32_t speed;
	uint32_t timeFly, timeRest;
	int res;

	res = sscanf(pLine, "%s can fly %u km/s for %u seconds, but then must rest for %u seconds.", rd, &speed, &timeFly, &timeRest);
	if (res != 4)
	{
		errLog(-1, "Could not parse line");
		return;
	}

	if (!reindeers.isMember(rd))
		reindeers[rd] = objectValue;

	reindeers[rd]["speed"] = speed;
	reindeers[rd]["timeFly"] = timeFly;
	reindeers[rd]["timeRest"] = timeRest;

	reindeers[rd]["timeLeft"] = timeFly;
	reindeers[rd]["state"] = 0;
	reindeers[rd]["distTraveled"] = 0;
	reindeers[rd]["points"] = 0;
}

uint32_t winnerGet(uint32_t time, uint32_t mode = 0)
{
	uint32_t t, timeLeft;
	Value::iterator rd;
	string name, nameFront;
	uint32_t state;
	uint32_t dist, distRes = 0;
	uint32_t points, pointsRes = 0;

	for (t = 0; t < time; ++t)
	{
		for (rd = reindeers.begin(); rd != reindeers.end(); ++rd)
		{
			if (!t)
			{
				(*rd)["timeLeft"] = (*rd)["timeFly"].asUInt();
				(*rd)["state"] = 0;
				(*rd)["distTraveled"] = 0;
				(*rd)["points"] = 0;
			}

			name = rd.key().asString();

			dist = (*rd)["distTraveled"].asUInt();
			state = (*rd)["state"].asUInt();

			if (!state)
			{
				dist += (*rd)["speed"].asUInt();
				(*rd)["distTraveled"] = dist;
			}

			timeLeft = (*rd)["timeLeft"].asUInt();
			//infLog("Reindeer %-15s time left %u in state %s", name.c_str(), timeLeft, state ? "resting" : "flying");

			--timeLeft;
			(*rd)["timeLeft"] = timeLeft;

			if (!timeLeft)
			{
				if (!state)
					(*rd)["timeLeft"] = (*rd)["timeRest"];
				else
					(*rd)["timeLeft"] = (*rd)["timeFly"];

				state ^= 1;
				//infLog("Reindeer %-15s is %s", name.c_str(), state ? "resting" : "flying");
				(*rd)["state"] = state;
			}

			if (!distRes or dist > distRes)
				distRes = dist;
		}

		for (rd = reindeers.begin(); rd != reindeers.end(); ++rd)
		{
			name = rd.key().asString();

			dist = (*rd)["distTraveled"].asUInt();
			if (dist < distRes)
				continue;

			points = (*rd)["points"].asUInt() + 1;
			(*rd)["points"] = points;

			if (!pointsRes or points > pointsRes)
				pointsRes = points;
		}
	}

	if (!mode)
		return distRes;
	else
		return pointsRes;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");
	//FastWriter writer;
	StyledWriter writer;
	string str;
	uint32_t pointsWinner;

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
	str = writer.write(reindeers);
	cout << str;
#endif
	pointsWinner = winnerGet(2503);
	procInfLog("Result1: %d", pointsWinner);

	pointsWinner = winnerGet(2503, 1);
	//pointsWinner = winnerGet(1000);
#if 0
	str = writer.write(reindeers);
	cout << str;
#endif
	procInfLog("Result2: %d", pointsWinner);

	return Positive;
}

