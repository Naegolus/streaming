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

#ifndef LIST_IDX_H
#define LIST_IDX_H

#include <cstdint>
#include <string>

class ListIdx
{

public:
	ListIdx();
	ListIdx(uint32_t win);
	virtual ~ListIdx();

	ListIdx &operator=(const uint32_t size);

	void winSet(uint32_t win);
	void sizeSet(uint32_t size);
	void sizeJumpSet(uint16_t size);

	uint32_t win() const;
	uint32_t size() const;

	bool keyProcess(uint8_t key);
	uint32_t cursor() const;
	uint32_t offset() const;

	uint32_t cursorAbs() const;
	bool endReached() const;
	bool winEndPrint(std::string &str, size_t idxRel, size_t idxAbs, uint8_t padding = 2) const;
	void cursorPrint(std::string &str, size_t idxRel, uint8_t padding = 2) const;

	void reset();

private:

	/*
	 * Naming of functions:  objectVerb()
	 * Example:              peerAdd()
	 */

	/* member functions */
	bool inc();
	bool dec();

	/* member variables */
	uint32_t mWin;
	uint32_t mSize;
	uint32_t mSizeJump;
	uint32_t mCursor;
	uint32_t mOffset;

	/* static functions */

	/* static variables */

	/* constants */

};

#endif

