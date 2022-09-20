/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 23.08.2022

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

#include "ConnectFourMatching.h"
#include "LibGaming.h"

#if 1
#define dGenCfMatchStateString(s) #s,
dProcessStateStr(CfMatchState);
#endif

using namespace std;
using namespace Json;

#define LOG_LVL	0

#define dColorTeam1		"92"
#define dColorTeam2		"96"

ConnectFourMatching::ConnectFourMatching()
	: Processing("ConnectFourMatching")
	, mState(CfMatchInit)
	, mStart(0)
	, mCntSec(0)
{}

/* member functions */
Success ConnectFourMatching::initialize()
{
	return Positive;
}

Success ConnectFourMatching::process()
{
	Value &gs = *pGs;
	uint32_t diff = millis() - mStart;
	bool res = false;

	switch (mState)
	{
	case CfMatchInit:

		matchInit();
		gamersInit();

		mState = CfMatchBeginStart;

		break;
	case CfMatchBeginStart:

		mCntSec = 1;
		frameBeginCreate();

		mStart = millis();
		mState = CfMatchBeginCntWait;

		break;
	case CfMatchBeginCntWait:

		if (diff < 1000)
			break;
		mStart = millis();

		--mCntSec;
		if (mCntSec)
		{
			frameBeginCreate();
			break;
		}

		mState = CfMatchRoundStart;

		break;
	case CfMatchRoundStart:

		mCntSec = 5;

		if (gs["match"]["teamCurrent"] == 1)
			gs["match"]["teamCurrent"] = 2;
		else
			gs["match"]["teamCurrent"] = 1;

		cursorsReset();

		gs["dirty"] = true;

		mStart = millis();
		mState = CfMatchRoundDoneWait;

		break;
	case CfMatchRoundDoneWait:

		msgProcess();
		framesRoundCreate();

		if (diff < 1000)
			break;
		mStart = millis();

		gs["dirty"] = true;

		--mCntSec;
		if (mCntSec)
			break;

		// Round finished
		roundResultAccept();

		res = matchFinished();
		if (!res)
		{
			mState = CfMatchRoundStart;
			break;
		}

		framesRoundCreate();

		mCntSec = 10;

		mStart = millis();
		mState = CfMatchStatsShow;

		break;
	case CfMatchStatsShow:

		framesRoundCreate();

		if (diff < 1000)
			break;
		mStart = millis();

		gs["dirty"] = true;

		--mCntSec;
		if (mCntSec)
			break;

		return Positive;

		break;
	default:
		break;
	}

	return Pending;
}

void ConnectFourMatching::matchInit()
{
	Value &gs = *pGs;

	gs["match"] = objectValue;
	gs["match"]["teamCurrent"] = 2;
	gs["match"]["teamCursor"] = 0;
	gs["match"]["tCursorCalcReq"] = false;
	gs["dirty"] = true;

	memset(mpBoard, 0, sizeof(mpBoard));
}

void ConnectFourMatching::gamersInit()
{
	Value &gs = *pGs;

	for (Value::iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		Value &g = *iter;

		g["cursor"] = 0;
		g["cursorSet"] = 0;

		if (g["team"].asUInt() > 2)
			g["team"] = 2;
	}
}

void ConnectFourMatching::frameBeginCreate()
{
	Value &gs = *pGs;

	Value msg;
	string frame;

	msgBegin(frame);

	msg["type"] = "frame";
	msg["data"] = frame;

	for (Value::const_iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		UInt64 id = stol(iter.key().asString());
		msg["gamers"].append(id);
	}

	(*pOut).commit(msg);
}

void ConnectFourMatching::msgBegin(string &str)
{
	str = "\e[2J\e[H";
	str += "\r\n";
	str += "The game starts in " + to_string(mCntSec) + "s!";
	str += "\r\n";
}

void ConnectFourMatching::msgProcess()
{
	PipeEntry<Value> msg;

	while ((*pIn).get(msg))
		msgInterpret(msg.particle);
}

void ConnectFourMatching::msgInterpret(const Value &msg)
{
#if 0
	FastWriter fastWriter;
	string str = fastWriter.write(msg);
	procInfLog("%s", str.c_str());
#endif

	Value &gs = *pGs;
	string type = msg["type"].asString();
	string id = msg["gamerId"].asString();

	if (type == "connect")
	{
		Value tmp;

		tmp["name"] = msg["gamerName"];
		tmp["team"] = 0;

		gs["gamers"][id] = tmp;
		gs["dirty"] = true;

		return;
	}

	if (type == "disconnect")
	{
		gs["gamers"].removeMember(id);
		gs["dirty"] = true;

		return;
	}

	Value &g = gs["gamers"][id];
	uint8_t team = g["team"].asUInt();
	uint8_t teamCurrent = gs["match"]["teamCurrent"].asUInt();

	if (team != teamCurrent)
		return;

	gamerMsgInterpret(msg);

	if (id != gs["admin"].asString())
		return;

	adminMsgInterpret(msg);
}

void ConnectFourMatching::gamerMsgInterpret(const Value &msg)
{
	string type = msg["type"].asString();

	if (type != "key")
		return;

	Value &gs = *pGs;
	string id = msg["gamerId"].asString();
	Value &g = gs["gamers"][id];
	uint8_t key = msg["key"].asUInt();
	uint8_t cursor = g["cursor"].asUInt();

	if (key == 'h' and cursor > 0)
	{
		g["cursor"] = cursor - 1;
		g["dirty"] = true;
	}

	if (key == 'l' and cursor < cCfBoardCols - 1)
	{
		g["cursor"] = cursor + 1;
		g["dirty"] = true;
	}

	if (key == 'j')
	{
		g["cursorSet"] = cursor;
		g["dirty"] = true;
		gs["match"]["tCursorCalcReq"] = true;
	}

	teamCursorCalc();
}

void ConnectFourMatching::adminMsgInterpret(const Value &msg)
{
	string type = msg["type"].asString();

	if (type != "key")
		return;

#if 0
	//Value &gs = *pGs;
	string id = msg["gamerId"].asString();
	uint8_t key = msg["key"].asUInt();
#endif
}

void ConnectFourMatching::teamCursorCalc()
{
	Value &gs = *pGs;

	if (!gs["match"]["tCursorCalcReq"].asBool())
		return;
	gs["match"]["tCursorCalcReq"] = false;

	uint8_t cursor = 0;
	uint32_t votes[cCfBoardCols];
	memset(votes, 0, sizeof(votes));

	uint8_t teamCurrent = gs["match"]["teamCurrent"].asUInt();
	uint8_t team = 0;

	for (Value::const_iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		const Value &g = *iter;

		team = g["team"].asUInt();
		if (team != teamCurrent)
			continue;

		cursor = g["cursorSet"].asUInt();
		++votes[cursor];
	}

	uint32_t votesMax = 0;
	uint8_t votesMaxCursor = 0;

	for (uint8_t i = 0; i < cCfBoardCols; ++i)
	{
		if (votes[i] <= votesMax)
			continue;

		votesMax = votes[i];
		votesMaxCursor = i;
	}

	if (votesMaxCursor != gs["match"]["teamCursor"].asUInt())
	{
		gs["match"]["teamCursor"] = votesMaxCursor;
		gs["dirty"] = true;
	}
}

void ConnectFourMatching::framesRoundCreate()
{
	Value &gs = *pGs;

	frmSpecCreate();
	frmTeamCurrentCreate();

	gs["dirty"] = false;
}

void ConnectFourMatching::frmSpecCreate()
{
	Value &gs = *pGs;

	if (!gs["dirty"].asBool())
		return;

	Value msg;
	string frame;
	uint8_t teamCurrent = gs["match"]["teamCurrent"].asUInt();
	uint8_t team = 0;

	msgBoard(frame);

	msg["type"] = "frame";
	msg["data"] = frame;

	for (Value::const_iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		const Value &g = *iter;

		team = g["team"].asUInt();
		if (team == teamCurrent)
			continue;

		UInt64 id = stol(iter.key().asString());
		msg["gamers"].append(id);
	}

	(*pOut).commit(msg);
}

void ConnectFourMatching::frmTeamCurrentCreate()
{
	Value &gs = *pGs;

	Value msg;
	string frame;
	uint8_t teamCurrent = gs["match"]["teamCurrent"].asUInt();
	uint8_t team = 0;
	bool needFrame = false;

	for (Value::iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		Value &g = *iter;

		team = g["team"].asUInt();
		if (team != teamCurrent)
			continue;

		needFrame = gs["dirty"].asBool() or g["dirty"].asBool();
		if (!needFrame)
			continue;
		g["dirty"] = false;

		msgBoard(frame, &g);

		msg = objectValue;
		msg["type"] = "frame";
		msg["data"] = frame;

		UInt64 id = stol(iter.key().asString());
		msg["gamers"].append(id);

		(*pOut).commit(msg);
	}
}

void ConnectFourMatching::cursorPrint(string &str, uint8_t cursor, uint8_t sign)
{
	str += "     ";
	for (uint8_t i = 0; i < cursor; ++i)
		str += "      ";
	str.push_back(sign);
}

void ConnectFourMatching::msgBoard(string &str, const Value *pGamer)
{
	Value &gs = *pGs;
	uint8_t cell = 0;
	uint8_t teamCurrent = gs["match"]["teamCurrent"].asUInt();
	uint8_t team = 0;
	uint8_t winner = 0;
	bool haveWinner = false;

	if (pGamer)
		team = (*pGamer)["team"].asUInt();

	if (gs["match"]["winner"])
	{
		winner = gs["match"]["winner"].asUInt();
		haveWinner = true;
	}

	str = "\e[2J\e[H";
	str += "\r\n";
	str += "Welcome to " + gs["name"].asString() + " Gaming()!";
	str += "\r\n";
	str += "Gamers " + to_string(gs["gamers"].size());
	str += "\r\n";
	str += "\r\n";
	if (pGamer and !haveWinner)
		cursorPrint(str, (*pGamer)["cursor"].asUInt(), 'V');
	str += "\r\n";

	if (haveWinner and winner == 1)
	{
		str += "             Team \e[1;";
		str += dColorTeam1;
		str += "mx\e[0m is the winner!";
	}
	else
	if (haveWinner and winner == 2)
	{
		str += "             Team \e[1;";
		str += dColorTeam2;
		str += "mo\e[0m is the winner!";
	}
	else
	if (haveWinner and !winner)
		str += "           Spectators are the winner!";
	else
	if (pGamer)
		cursorPrint(str, (*pGamer)["cursorSet"].asUInt(), 'Y');

	str += "\r\n";
	if (!haveWinner)
		cursorPrint(str, gs["match"]["teamCursor"].asUInt(), 'T');
	str += "\r\n";
	str += "  /-----------------------------------------\\";
	str += "\r\n";

	for (size_t r = 0; r < cCfBoardRows; ++r)
	{
		str += "  |";

		for (size_t c = 0; c < cCfBoardCols; ++c)
		{
			str += "  ";

			cell = mpBoard[c][cCfBoardRows - 1 - r];

			if (cell == 1)
			{
				str += "\e[1;";
				str += dColorTeam1;
				str += "mx";
			}
			else
			if (cell == 2)
			{
				str += "\e[1;";
				str += dColorTeam2;
				str += "mo";
			}
			else
				str += " ";

			str += "\e[0m  |";
		}

		str += "\r\n";

		if (r < cCfBoardRows - 1)
			str += "  |-----+-----+-----+-----+-----+-----+-----|\r\n";
		else
			str += "  \\-----------------------------------------/\r\n";
	}

	if (haveWinner)
		teamCurrent = 0;

	str += "\r\n";
	str += "Current team: ";

	if (teamCurrent == 1)
	{
		str += "\e[1;";
		str += dColorTeam1;
		str += "mx\e[0m";
	}
	else
	if (teamCurrent == 2)
	{
		str += "\e[1;";
		str += dColorTeam2;
		str += "mo\e[0m";
	}
	else
		str += "-";

	if (team == teamCurrent)
	{
		str += " (\e[";

		if (team == 1)
			str += dColorTeam1;
		else
			str += dColorTeam2;

		str += ";1mYou\e[0m)";
	} else
		str += "     ";

	str += "                 [?] Help";

	str += "\r\n";
	str += "Round time left: " + to_string(mCntSec) + "s";
	str += "\r\n";
}

void ConnectFourMatching::cursorsReset()
{
	Value &gs = *pGs;

	gs["match"]["teamCursor"] = 0;

	for (Value::iterator iter = gs["gamers"].begin(); iter != gs["gamers"].end(); ++iter)
	{
		Value &g = *iter;

		uint8_t team = g["team"].asUInt();
		if (!team)
			continue;

		g["cursor"] = 0;
		g["cursorSet"] = 0;
	}
}

void ConnectFourMatching::roundResultAccept()
{
	Value &gs = *pGs;
	uint8_t team = gs["match"]["teamCurrent"].asUInt();
	uint8_t cursor = gs["match"]["teamCursor"].asUInt();
	uint8_t *pCol = &mpBoard[cursor][0];

	for (uint8_t i = 0; i < cCfBoardRows; ++i, ++pCol)
	{
		if (*pCol)
			continue;

		*pCol = team;
		break;
	}
}

uint8_t ConnectFourMatching::boardLineCheck(uint8_t col, uint8_t row, int8_t dcol, int8_t drow)
{
	uint8_t teamStart = mpBoard[col][row];

	for (uint8_t i = 0; i < cCfNumToWin - 1; ++i)
	{
		col += dcol;
		row += drow;

		if (col < 0 or col >= cCfBoardCols)
			return 0;

		if (row < 0 or row >= cCfBoardRows)
			return 0;

		if (mpBoard[col][row] != teamStart)
			return 0;
	}

	return teamStart;
}

uint8_t ConnectFourMatching::boardStarCheck(uint8_t col, uint8_t row)
{
	uint8_t winner = 0;

	if (row > cCfBoardRows - cCfNumToWin)
		return 0;

	winner = boardLineCheck(col, row, -1, 1);
	if (winner)
		return winner;

	winner = boardLineCheck(col, row,  0, 1);
	if (winner)
		return winner;

	winner = boardLineCheck(col, row,  1, 1);
	if (winner)
		return winner;

	winner = boardLineCheck(col, row,  1, 0);
	if (winner)
		return winner;

	return 0;
}

bool ConnectFourMatching::matchFinished()
{
	Value &gs = *pGs;
	bool spaceLeft = false;
	uint8_t winner = 0;

	for (uint8_t col = 0; col < cCfBoardCols; ++col)
	{
		for (uint8_t row = 0; row < cCfBoardRows; ++row)
		{
			if (!mpBoard[col][row])
				spaceLeft = true;

			winner = boardStarCheck(col, row);
			if (!winner)
				continue;

			gs["match"]["winner"] = winner;
			return true;
		}
	}

	if (!spaceLeft)
	{
		gs["match"]["winner"] = 0;
		return true;
	}

	return false;
}

void ConnectFourMatching::processInfo(char *pBuf, char *pBufEnd)
{
#if 1
	dInfo("State\t\t\t%s\n", CfMatchStateString[mState]);
#endif
}

/* static functions */

