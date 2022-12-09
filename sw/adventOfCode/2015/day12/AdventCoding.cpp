
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>
#include "AdventCoding.h"

using namespace std;
using namespace Json;

char line[10000];

Value val;

int32_t numSum(const Value &v, bool redFilter = false, bool inArray = true)
{
	Value::const_iterator iter;
	int32_t sum = 0;

	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		//infLog("Checking %s", iter.key().asString().c_str());

		if (iter->isObject())
		{
			//infLog("Object");
			sum += numSum(*iter, redFilter, false);
			continue;
		}

		if (iter->isArray())
		{
			//infLog("Array");
			sum += numSum(*iter, redFilter, true);
			continue;
		}

		if (iter->isNumeric())
		{
			//infLog("Number");
			sum += iter->asInt();
			continue;
		}

		if (iter->isString())
		{
			if (redFilter and !inArray and iter->asString() == "red")
			{
				//wrnLog("Ignoring red");
				return 0;
			}

			//infLog("String");
			continue;
		}

		//errLog(-1, "Not processed");
	}

	return sum;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input.txt", "r");
	int32_t sum = 0;

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

	}

	ifstream f("../input2.txt");
	f >> val;
#if 0
	StyledWriter writer;
	cout << writer.write(val);
#endif
	fclose(pFile);
	pFile = NULL;

	sum = numSum(val);
	procInfLog("Result1: %d", sum);

	sum = numSum(val, true, val.isArray());
	procInfLog("Result2: %d", sum);

	return Positive;
}

