/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 31.03.2018

  Copyright (C) 2018-now Authors and www.dsp-crowd.com

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

#ifndef ENV_H
#define ENV_H

#include <mutex>

#include <jsoncpp/json/json.h>
using namespace Json;

#if 0
#include "resources.h"
#include "Res.h"
#endif

/*
 * ##################################
 * Environment contains variables for
 *          !! IPC ONLY !!
 * ##################################
 */
extern std::mutex envMtx;
extern Value env;

#endif

