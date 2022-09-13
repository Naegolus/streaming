/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 22.08.2022

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

#ifndef LIB_GAMING_H
#define LIB_GAMING_H

#include "TcpTransfering.h"

const uint8_t keyBackspace = 0x7F;
const uint8_t keyEnter = 0x0D;
const uint8_t keyEsc = 0x1B;

const uint8_t cNameSizeMin = 2;
const uint8_t cNameSizeMax = 16;

uint8_t keyGet(TcpTransfering *pConn, uint32_t lastGotMs);
bool keyIsAlphaNum(uint8_t key);
bool keyIsNum(uint8_t key);
bool keyIsCommon(uint8_t key);

#endif

