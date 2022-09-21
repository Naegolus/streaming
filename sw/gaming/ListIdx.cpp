/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 15.09.2022

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

#include "ListIdx.h"

using namespace std;

ListIdx::ListIdx()
	: mWin(0)
	, mSize(0)
	, mCursor(0)
	, mOffset(0)
{
}

ListIdx::ListIdx(uint32_t win)
	: mWin(win)
	, mSize(0)
	, mSizeJump(5)
	, mCursor(0)
	, mOffset(0)
{
}

ListIdx::~ListIdx()
{
}

ListIdx &ListIdx::operator=(const uint32_t size)
{
	sizeSet(size);
	return *this;
}

void ListIdx::winSet(uint32_t win)
{
	mWin = win;
	reset();
}

void ListIdx::sizeSet(uint32_t size)
{
	mSize = size;
	reset();
}

void ListIdx::sizeJumpSet(uint16_t size)
{
	mSizeJump = size;
}

uint32_t ListIdx::win() const
{
	return mWin;
}

uint32_t ListIdx::size() const
{
	return mSize;
}

bool ListIdx::keyProcess(uint8_t key)
{
	bool changed = false;

	if (key == 'j' and inc())
		return true;

	if (key == 'J')
	{
		for (uint16_t i = 0; i < mSizeJump; ++i)
			changed |= inc();

		return changed;
	}

	if (key == 'k' and dec())
		return true;

	if (key == 'K')
	{
		for (uint16_t i = 0; i < mSizeJump; ++i)
			changed |= dec();

		return changed;
	}

	if (key == 'g')
	{
		changed = !mCursor;
		reset();
		return changed;
	}

	return false;
}

bool ListIdx::inc()
{
	if (mCursor >= mSize - 1)
		return false;

	if (mCursor >= mWin - 1)
		return false;

	if ((mCursor == mWin / 2) and (mOffset + mWin < mSize))
		++mOffset;
	else
		++mCursor;

	return true;
}

bool ListIdx::dec()
{
	if (!mCursor)
		return false;

	if ((mCursor == mWin / 2) and mOffset)
		--mOffset;
	else
		--mCursor;

	return true;
}

uint32_t ListIdx::cursor() const
{
	return mCursor;
}

uint32_t ListIdx::offset() const
{
	return mOffset;
}

uint32_t ListIdx::cursorAbs() const
{
	return mOffset + mCursor;
}

bool ListIdx::endReached() const
{
	return mOffset + mWin >= mSize;
}

void ListIdx::reset()
{
	mCursor = 0;
	mOffset = 0;
}

