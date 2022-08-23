/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 23.08.2022

  Copyright (C) 2022 Authors and www.dsp-crowd.com

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

#include "TicTacToeMatching.h"

using namespace std;

#define LOG_LVL	0

TicTacToeMatching::TicTacToeMatching()
	: Processing("TicTacToeMatching")
{}

/* member functions */
Success TicTacToeMatching::initialize()
{
	return Positive;
}

Success TicTacToeMatching::process()
{
	return Pending;
}

Success TicTacToeMatching::shutdown()
{
	return Positive;
}

void TicTacToeMatching::processInfo(char *pBuf, char *pBufEnd)
{
	(void)pBuf;
	(void)pBufEnd;
}

/* static functions */

