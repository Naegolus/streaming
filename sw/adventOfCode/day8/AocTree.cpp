/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 08.12.2022

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

#include <iostream>
#include "AocTree.h"

using namespace std;

uint32_t sizeFile = 0;
uint32_t cntCols = 0;
uint32_t cntRows = 0;

struct Tree
{
	uint8_t height;
};

Tree *pTrees = NULL;
uint32_t currRow = 0;

#define idx(r, c)		(r * cntCols + c)

void lineParse(const char *pLine)
{
	if (!pTrees)
	{
		cntCols = strlen(pLine);
		cntRows = sizeFile / (cntCols + 1);
		infLog("Number of columns: %d", cntCols);
		infLog("Number of rows:    %d", cntRows);

		uint32_t sizeAlloc = sizeof(*pTrees) * cntCols * cntRows;
		infLog("Allocation size:   %d", sizeAlloc);
		pTrees = (Tree *)calloc(1, sizeAlloc);
	}

	uint32_t c;
	const char *pCh = pLine;

	for (c = 0; c < strlen(pLine); ++c, ++pCh)
		pTrees[idx(currRow, c)].height = *pCh - '0';

	++currRow;
}

void treesPrint()
{
	uint32_t r, c;

	for (r = 0; r < cntRows; ++r)
	{
		for (c = 0; c < cntCols; ++c)
		{
			if (c)
				cout << ", ";
			cout << (uint32_t)pTrees[idx(r, c)].height;
		}
		cout << endl;
	}
}

uint32_t treeVisible(uint32_t r, uint32_t c)
{
	bool vis;
	uint32_t i;
	uint32_t hRef, h;

	hRef = pTrees[idx(r, c)].height;

	// horizontal
	vis = true;
	for (i = 0; i < c; ++i)
	{
		h = pTrees[idx(r, i)].height;
		if (h >= hRef)
		{
			vis = false;
			break;
		}
	}
	if (vis) return 1;

	vis = true;
	for (i = c + 1; i < cntCols; ++i)
	{
		h = pTrees[idx(r, i)].height;
		if (h >= hRef)
		{
			vis = false;
			break;
		}
	}
	if (vis) return 1;

	// vertical
	vis = true;
	for (i = 0; i < r; ++i)
	{
		h = pTrees[idx(i, c)].height;
		if (h >= hRef)
		{
			vis = false;
			break;
		}
	}
	if (vis) return 1;

	vis = true;
	for (i = r + 1; i < cntRows; ++i)
	{
		h = pTrees[idx(i, c)].height;
		if (h >= hRef)
		{
			vis = false;
			break;
		}
	}
	if (vis) return 1;

	return 0;
}

uint32_t treeScenic(uint32_t r, uint32_t c)
{
	int32_t i;
	uint32_t hRef, h;
	uint32_t scenicL = 0;
	uint32_t scenicR = 0;
	uint32_t scenicT = 0;
	uint32_t scenicB = 0;

	hRef = pTrees[idx(r, c)].height;

	// horizontal
	for (i = c - 1; c and i >= 0; --i)
	{
		++scenicL;

		h = pTrees[idx(r, i)].height;
		if (h >= hRef)
			break;
	}

	for (i = c + 1; i < (int32_t)cntCols; ++i)
	{
		if (c == cntCols - 1)
			break;

		++scenicR;

		h = pTrees[idx(r, i)].height;
		if (h >= hRef)
			break;
	}

	// vertical
	for (i = r - 1; r and i >= 0; --i)
	{
		++scenicT;

		h = pTrees[idx(i, c)].height;
		if (h >= hRef)
			break;
	}

	for (i = r + 1; i < (int32_t)cntRows; ++i)
	{
		++scenicB;

		h = pTrees[idx(i, c)].height;
		if (h >= hRef)
			break;
	}

	return scenicL * scenicR * scenicT * scenicB;
}

