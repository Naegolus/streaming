/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  File created on 21.05.2019

  Copyright (C) 2019-now Authors and www.dsp-crowd.com

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

#ifndef TCP_TRANSFERING_H
#define TCP_TRANSFERING_H

#include <string>

#include "Processing.h"

class TcpTransfering : public Processing
{

public:

	static TcpTransfering *create(int fd)
	{
		return new (std::nothrow) TcpTransfering(fd);
	}

	static TcpTransfering *create(const std::string &addr)
	{
		return new (std::nothrow) TcpTransfering(addr);
	}

	bool usable();
	ssize_t read(void *pBuf, size_t len);
	ssize_t readFlush();
	void send(const void *pData, size_t len);

protected:

	TcpTransfering() : Processing("TcpTransfering") {}
	TcpTransfering(int fd);
	TcpTransfering(const std::string &addr);
	virtual ~TcpTransfering() {}

private:

	TcpTransfering(const TcpTransfering &) : Processing("") {}
	TcpTransfering &operator=(const TcpTransfering &)
	{
		return *this;
	}

	Success initialize();
	Success process();
	Success shutdown();
	void disconnect(int err = 0);

	void processInfo(char *pBuf, char *pBufEnd);

	std::mutex mSocketFdMtx;
	int mSocketFd;
	int mErrno;
	bool mUsable;

	// statistics
	uint32_t mBytesReceived;
	uint32_t mBytesSent;

};

#endif

