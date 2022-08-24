/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 19.03.2021

  Copyright (C) 2021-now Authors and www.dsp-crowd.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <iostream>
#include <chrono>
#include <stdarg.h>
#if CONFIG_PROC_HAVE_DRIVERS
#include <mutex>
#endif

#include "Pipe.h"

#include "env.h"

#define dLogEntryBufferSize		1024

using namespace std;
using namespace std::chrono;
using namespace Json;

#if CONFIG_PROC_HAVE_DRIVERS
static mutex mtxPrint;
#endif

static system_clock::time_point tOld;

const string red("\033[0;31m");
const string yellow("\033[0;33m");
const string reset("\033[0m");

Pipe<Value> ppGlobLogEntries;

int16_t logEntryCreate(const int severity, const char *filename, const char *function, const int line, const int16_t code, const char *msg, ...)
{
#if CONFIG_PROC_HAVE_DRIVERS
	lock_guard<mutex> lock(mtxPrint);
#endif

	char *pBuf = new (nothrow) char[dLogEntryBufferSize];
	if (!pBuf)
		return code;

	char *pStart = pBuf;
	char *pEnd = pStart + dLogEntryBufferSize;

	va_list args;

	system_clock::time_point t = system_clock::now();
	duration<long, nano> tDiff = t - tOld;
	float tDiffSec = tDiff.count() / 10e9;
	time_t tt_t = system_clock::to_time_t(t);
	tm *tm_t = ::localtime(&tt_t);
	char timeBuf[32];

	tOld = t;

	strftime(timeBuf, sizeof(timeBuf), "%d%m%y %H:%M:%S", tm_t);

	// "%03d"
	pStart += snprintf(pStart, pEnd - pStart, "%s.000 +%3.3f %4d %3d  %-24s ", timeBuf, tDiffSec, line, severity, function);

	va_start(args, msg);
	pStart += vsnprintf(pStart, pEnd - pStart, msg, args);
	va_end(args);

	// Creating log entry
	int logLevel = env["args"]["verbosity"].asInt();
	if (severity <= logLevel)
	{
		if (severity == 1)
		{
			cerr << red << pBuf << reset << endl;
			cerr.flush();
		}
		else
		if (severity == 2)
		{
			cerr << yellow << pBuf << reset << endl;
			cerr.flush();
		}
		else
		{
			cout << pBuf << endl;
			cout.flush();
		}
	}

	Value entry;

	entry["msg"] = pBuf;

	ppGlobLogEntries.commit(entry);
	ppGlobLogEntries.toPushTry();

	delete[] pBuf;

	return code;
}

