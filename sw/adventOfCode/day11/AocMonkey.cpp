
#include <iostream>
#include <sstream>

#include "AocMonkey.h"
#include "Processing.h"

using namespace std;
using namespace Json;

Value notes;
string idMonkeyCurr;

uint32_t superMod = 1;

void lineParse(char *pLine)
{
	int res;
	const char *pCheck = NULL;
	uint32_t lenCheck = 0;
	char bufValL[8];
	char bufValR[8];
	char bufOp[4];

	if (!strlen(pLine))
		return;

	if (pLine[strlen(pLine) - 1] == ':')
		pLine[strlen(pLine) - 1] = 0;

	res = sscanf(pLine, "Monkey %s", bufValR);
	if (res == 1)
	{
		if (notes.isMember(bufValR))
		{
			errLog(-1, "Could not add monkey. ID %s exists already", bufValR);
			return;
		}

		//infLog("Adding monkey %s", bufValR);
		notes[bufValR] = objectValue;

		idMonkeyCurr = bufValR;

		return;
	}

	if (!idMonkeyCurr.size())
	{
		errLog(-1, "Current monkey not set");
		return;
	}

	pCheck = "  Starting items: ";
	lenCheck = strlen(pCheck);
	if (!strncmp(pLine, pCheck, lenCheck))
	{
		pLine += lenCheck;

		stringstream ss(pLine);
		string strItem;
		uint32_t item;

		notes[bufValR]["starting"] = arrayValue;

		//infLog("Adding list: %s", pLine);

		while (getline(ss, strItem, ','))
		{
			if (strItem.front() == ' ')
				strItem = strItem.substr(1);

			//cout << strItem << endl;

			item = stoi(strItem);
			//cout << item << endl;

			notes[idMonkeyCurr]["starting"].append(item);
			notes[idMonkeyCurr]["cntThrown"] = 0;
		}

		return;
	}

	res = sscanf(pLine, "  Operation: new = %s %s %s", bufValL, bufOp, bufValR);
	if (res == 3)
	{
		//infLog("Adding operation: %s %s %s", bufValL, bufOp, bufValR);

		notes[idMonkeyCurr]["op"] = objectValue;
		notes[idMonkeyCurr]["op"]["valL"] = bufValL;
		notes[idMonkeyCurr]["op"]["valR"] = bufValR;
		notes[idMonkeyCurr]["op"]["op"] = bufOp;

		return;
	}

	res = sscanf(pLine, "  Test: divisible by %s", bufValR);
	if (res == 1)
	{
		//infLog("Adding test: %s", bufValR);

		notes[idMonkeyCurr]["test"] = objectValue;
		notes[idMonkeyCurr]["test"]["val"] = atoi(bufValR);

		return;
	}

	res = sscanf(pLine, "    If true: throw to monkey %s", bufValR);
	if (res == 1)
	{
		//infLog("Adding true case: %s", bufValR);

		notes[idMonkeyCurr]["test"]["true"] = bufValR;

		return;
	}

	res = sscanf(pLine, "    If false: throw to monkey %s", bufValR);
	if (res == 1)
	{
		//infLog("Adding false case: %s", bufValR);

		notes[idMonkeyCurr]["test"]["false"] = bufValR;

		return;
	}

	errLog(-1, "Could not parse line");
	//infLog("Line: %s", pLine);
}

void superModCalc()
{
	uint32_t idMonkey, numMonkeys = notes.size();
	uint32_t val;

	for (idMonkey = 0; idMonkey < numMonkeys; ++idMonkey)
	{
		val = notes[to_string(idMonkey)]["test"]["val"].asUInt();
		//infLog("Using: %u", val);
		superMod *= val;
	}

	infLog("Supermod: %u", superMod);
}

void monkeyTurnExec(uint32_t id, bool noWorry = true)
{
	string strId = to_string(id);
	Value &monkey = notes[strId];
	//uint32_t item;
	uint32_t numItem;

	numItem = monkey["starting"].size();
	if (!numItem)
	{
		//infLog("Monkey %s is doing nothing", strId.c_str());
		return;
	}

	string strValR, strOp, idMonkeyTarget;
	uint64_t worry;
	uint64_t valR, mod;

	//infLog("Monkey %s:", strId.c_str());

	while (numItem)
	{
		worry = monkey["starting"][0].asUInt64();
		monkey["starting"].removeIndex(0, NULL);

		//infLog("  Inspecting item with a worry level of %lu", worry);

		if (!noWorry)
			worry %= superMod;

		strValR = monkey["op"]["valR"].asString();
		if (strValR == "old")
			valR = worry;
		else
			valR = stoi(strValR);

		strOp = monkey["op"]["op"].asString();
		if (strOp == "+")
		{
			worry += valR;
			//infLog("    Worry level is increases by %lu to %lu.", valR, worry);
		}
		else
		if (strOp == "*")
		{
			worry *= valR;
			//infLog("    Worry level is multiplied by %lu to %lu.", valR, worry);
		}
		else
		{
			errLog(-1, "    Unknown operation %s", strOp.c_str());
			return;
		}

		if (noWorry)
		{
			worry /= 3;
#if 0
			infLog("    Monkey gets bored with item. Worry level is divided by 3 to %u.",
					worry);
#endif
		}

		valR = monkey["test"]["val"].asUInt();
		mod = worry % valR;

		if (!mod)
			idMonkeyTarget = monkey["test"]["true"].asString();
		else
			idMonkeyTarget = monkey["test"]["false"].asString();

		//infLog("    Current worry level is %sdivisible by %u.", mod ? "not " : "", valR);
#if 0
		infLog("    Item with worry level %lu is thrown to monkey %s.",
						worry,
						idMonkeyTarget.c_str());
#endif
		notes[idMonkeyTarget]["starting"].append(worry);

		monkey["cntThrown"] = monkey["cntThrown"].asUInt() + 1;
		--numItem;
	}
}

void numRoundsExec(uint32_t numRounds, bool noWorry)
{
	uint32_t round;
	uint32_t idMonkey, numMonkeys = notes.size();

	for (round = 0; round < numRounds; ++round)
	{
		//cout << "##############" << endl;
		//monkeyItemsPrint();
		//cout << "-------" << endl;

		for (idMonkey = 0; idMonkey < numMonkeys; ++idMonkey)
			monkeyTurnExec(idMonkey, noWorry);

		//monkeyItemsPrint();
		//cout << "-------" << endl;
		//monkeyCntThrownPrint();
	}
}

void monkeyItemsPrint()
{
	uint32_t idMonkey, numMonkeys = notes.size();
	string strId;
	uint32_t item, numItems;

	for (idMonkey = 0; idMonkey < numMonkeys; ++idMonkey)
	{
		strId = to_string(idMonkey);

		cout << "Monkey " << idMonkey << ": ";

		numItems = notes[strId]["starting"].size();
		for (item = 0; item < numItems; ++item)
		{
			if (item)
				cout << ", ";
			cout << notes[strId]["starting"][item].asUInt();
		}

		cout << endl;
	}
}

uint64_t monkeyCntThrownPrint()
{
	uint32_t idMonkey, numMonkeys = notes.size();
	string strId;

	for (idMonkey = 0; idMonkey < numMonkeys; ++idMonkey)
	{
		strId = to_string(idMonkey);

		cout << "Monkey " << idMonkey;
		cout << " inspected items " << notes[strId]["cntThrown"].asUInt();
		cout << " times." << endl;
	}

	return notes["0"]["cntThrown"].asUInt64() *
		notes[to_string(numMonkeys - 1)]["cntThrown"].asUInt64();
}

