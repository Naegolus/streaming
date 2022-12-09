/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 06.12.2022

  Copyright (C) 2022-now Authors and www.dsp-crowd.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <jsoncpp/json/json.h>

#include "Processing.h"
#include "Circuit.h"

using namespace std;
using namespace Json;

Value wires;

uint32_t cntCheck = 0;
uint32_t cntCheckMax = 400000;

void assignmentAdd(char *pCmd)
{
	int res;
	char bufSrc[16];
	char bufDst[16];

	res = sscanf(pCmd, "%s -> %s", bufSrc, bufDst);
	if (res != 2)
		return;

	//infLog("Assignment");

	if (wires.isMember(bufDst))
	{
		errLog(-1, "Destination already assigned");
		return;
	}

	wires[bufDst] = objectValue;
	wires[bufDst]["cmd"] = "assign";
	wires[bufDst]["in1"] = bufSrc;
#if 0
	FastWriter fastWriter;
	string str = fastWriter.write(wires[bufDst]);
	infLog("%s", str.c_str());
#endif
}

void notAdd(char *pCmd)
{
	int res;
	char bufSrc[16];
	char bufDst[16];

	res = sscanf(pCmd, "NOT %s -> %s", bufSrc, bufDst);
	if (res != 2)
		return;

	//infLog("NOT");

	if (wires.isMember(bufDst))
	{
		errLog(-1, "Destination already assigned");
		return;
	}

	wires[bufDst] = objectValue;
	wires[bufDst]["cmd"] = "NOT";
	wires[bufDst]["in1"] = bufSrc;
#if 0
	FastWriter fastWriter;
	string str = fastWriter.write(wires[bufDst]);
	infLog("%s", str.c_str());
#endif
}

void gateAdd(char *pCmd)
{
	int res;
	char bufIn1[16];
	char bufGate[16];
	char bufIn2[16];
	char bufDst[16];

	res = sscanf(pCmd, "%s %s %s -> %s", bufIn1, bufGate, bufIn2, bufDst);
	if (res != 4)
		return;

	//infLog("GATE");

	if (wires.isMember(bufDst))
	{
		errLog(-1, "Destination already assigned");
		return;
	}

	wires[bufDst] = objectValue;
	wires[bufDst]["cmd"] = bufGate;
	wires[bufDst]["in1"] = bufIn1;
	wires[bufDst]["in2"] = bufIn2;
#if 0
	FastWriter fastWriter;
	string str = fastWriter.write(wires[bufDst]);
	infLog("%s", str.c_str());
#endif
}

uint16_t wireSig(Value &wire, uint16_t depth)
{
	string cmd;
	string in1, in2;
	uint16_t val1, val2, val;
	FastWriter fastWriter;
	string str = fastWriter.write(wire);

	if (str.back() == '\n')
		str.pop_back();

	++depth;

	infLog("Processing cmd. depth = %d", depth);
	infLog("%s", str.c_str());

	++cntCheck;
	if (cntCheck > cntCheckMax)
	{
		errLog(-1, "Check counter overflow");
		return 0;
	}

	if (wire.isMember("cache"))
	{
		val = wire["cache"].asUInt();
		return val;
	}

	wrnLog("Calculating value");

	cmd = wire["cmd"].asString();
	in1 = wire["in1"].asString();
	if (wire.isMember("in2"))
		in2 = wire["in2"].asString();

	if (wires.isMember(in1))
		val1 = wireSig(wires[in1], depth);
	else
		val1 = atoi(in1.c_str());

	if (wires.isMember(in2))
		val2 = wireSig(wires[in2], depth);
	else
		val2 = atoi(in2.c_str());

	if (cmd == "assign")
		val = val1;

	if (cmd == "OR")
		val = val1 | val2;

	if (cmd == "AND")
		val = val1 & val2;

	if (cmd == "LSHIFT")
		val = val1 << val2;

	if (cmd == "RSHIFT")
		val = val1 >> val2;

	if (cmd == "NOT")
		val = ~val1;

	wrnLog("Caching: %d", val);
	wire["cache"] = val;

	return val;
}

bool wireAssign(const char *pWire, uint16_t val)
{
	if (!wires.isMember(pWire))
		return false;

	wires[pWire]["cmd"] = "assign";
	wires[pWire]["in1"] = to_string(val);

	return true;
}

uint16_t wireSig(const char *pWire, uint16_t depth)
{
	infLog("Processing %s. depth = %d", pWire, depth);
	infLog("Number of wires: %d", wires.size());
#if 0
	Value::Members members = wires.getMemberNames();

	for (vector<string>::iterator iter = members.begin();
		iter != members.end(); ++iter)
	{
		infLog("Wire: %s", iter->c_str());
	}
#endif
	return wireSig(wires[pWire], depth);
}

