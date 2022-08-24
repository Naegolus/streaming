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

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif
#include <unistd.h>

#include "TcpTransfering.h"

using namespace std;

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

#define LOG_LVL	0

/*
 * Literature
 * - https://stackoverflow.com/questions/28027937/cross-platform-sockets
 */
TcpTransfering::TcpTransfering(int fd)
	: Processing("TcpTransfering")
	, mSocketFd(fd)
	, mErrno(0)
	, mUsable(false)
	, mBytesReceived(0)
	, mBytesSent(0)
{
}

TcpTransfering::TcpTransfering(const string &addr)
	: Processing("TcpTransfering")
	, mSocketFd(-1)
	, mErrno(0)
	, mUsable(false)
	, mBytesReceived(0)
	, mBytesSent(0)
{
	// TODO: Implement "TcpConnecting()"
}

Success TcpTransfering::initialize()
{
	lock_guard<mutex> lock(mSocketFdMtx);

	int opt = 1;

	if (mSocketFd < 0)
		return procErrLog(-1, "socket file descriptor not set");

	if (::setsockopt(mSocketFd, SOL_SOCKET, SO_KEEPALIVE, (const char *)&opt, sizeof(opt)))
		return procErrLog(-2, "setsockopt(SO_KEEPALIVE) failed: %s", strerror(errno));

#ifdef _WIN32
	unsigned long nonBlockMode = 1;

	opt = ioctlsocket(mSocketFd, FIONBIO, &nonBlockMode);
	if (opt == SOCKET_ERROR)
		return procErrLog(-3, "setsockopt(SO_KEEPALIVE) failed: %s", strerror(errno));
#else
	opt = fcntl(mSocketFd, F_GETFL, 0);
	if (opt == -1)
		return procErrLog(-3, "fcntl(F_GETFL) failed: %s", strerror(errno));

	opt |= O_NONBLOCK;

	opt = fcntl(mSocketFd, F_SETFL, opt);
	if (opt == -1)
		return procErrLog(-4, "fcntl(F_SETFL, 0x%08X) failed: %s", opt, strerror(errno));
#endif

	return Positive;
}

Success TcpTransfering::process()
{
	ssize_t connCheck = read(NULL, 0);

	if (connCheck >= 0)
		return Pending;

	if (mErrno)
		return procErrLog(-1, "connection error occured: %s (%d)", strerror(mErrno), mErrno);

	return Positive;
}

Success TcpTransfering::shutdown()
{
	procDbgLog(LOG_LVL, "shutdown");

	lock_guard<mutex> lock(mSocketFdMtx);
	disconnect();

	return Positive;
}

bool TcpTransfering::usable()
{
	return false;
}

/*
Literature socket programming:
- http://man7.org/linux/man-pages/man2/poll.2.html
- http://man7.org/linux/man-pages/man2/recvmsg.2.html
- http://man7.org/linux/man-pages/man2/select.2.html
- http://man7.org/linux/man-pages/man2/read.2.html
- https://linux.die.net/man/2/send
  - Important: MSG_NOSIGNAL
- https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_74/rzab6/poll.htm
- https://docs.microsoft.com/en-us/windows/desktop/winsock/complete-server-code
- https://stackoverflow.com/questions/28027937/cross-platform-sockets
- https://daniel.haxx.se/docs/poll-vs-select.html
- http://deepix.github.io/2016/10/21/tcprst.html
- https://stackoverflow.com/questions/11436013/writing-to-a-closed-local-tcp-socket-not-failing
- https://www.usenix.org/legacy/publications/library/proceedings/usenix99/full_papers/banga/banga_html/node3.html
- https://www.linuxtoday.com/blog/multiplexed-i0-with-poll.html
- https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/poll.h
- https://stackoverflow.com/questions/24791625/how-to-handle-the-linux-socket-revents-pollerr-pollhup-and-pollnval
*/
/*
 * Return value
 *   > 0 number of bytes read
 *   = 0 no data at the moment, but data can be expected in the future
 *   < 0 no data can be expected in the future
 */
ssize_t TcpTransfering::read(void *pBuf, size_t len)
{
	lock_guard<mutex> lock(mSocketFdMtx);

	if (!initDone())
		return 0;

	if (mSocketFd < 0)
		return -1;

	ssize_t numBytes = 0;
	bool peek = false;
	char buf[1];

	if (!pBuf or !len)
	{
		pBuf = buf;
		len = sizeof(buf);
		peek = true;
	}

	numBytes = ::recv(mSocketFd, (char *)pBuf, len, MSG_PEEK);

	if (numBytes < 0)
	{
		if (errno == EAGAIN or errno == EWOULDBLOCK)
			return 0; // std case and ok

		disconnect(errno);

		return procErrLog(-2, "recv() failed: %s", strerror(errno));
	}

	if (!numBytes)
	{
		procDbgLog(LOG_LVL, "connection reset by peer");
		disconnect();
		return -3;
	}

	if (peek)
		return numBytes;

	numBytes = ::recv(mSocketFd, (char *)pBuf, numBytes, 0);
	//procDbgLog(LOG_LVL, "received data. len: %d", numBytes);

	mBytesReceived += numBytes;

	return numBytes;
}

ssize_t TcpTransfering::readFlush()
{
	ssize_t bytesRead = 1, bytesSum = 0;
	char buf[32];

	while (bytesRead)
	{
		bytesRead = read(buf, sizeof(buf));
		bytesSum += bytesRead;
	}

	return bytesSum;
}

void TcpTransfering::send(const void *pData, size_t len)
{
	lock_guard<mutex> lock(mSocketFdMtx);

	if (mSocketFd < 0)
		return;

	ssize_t res;
	size_t lenBkup = len;
	size_t bytesSent = 0;

	while (len)
	{
		/* IMPORTANT:
		  * Connection may be reset by remote peer already.
		  * Flag MSG_NOSIGNAL prevents function send() to
		  * emit signal SIGPIPE and therefore kill the entire
		  * application in this case.
		  */
		res = ::send(mSocketFd, (const char *)pData, len, MSG_NOSIGNAL);

		if (res < 0)
		{
			procDbgLog(LOG_LVL, "connection down: %s", strerror(errno));
			disconnect(errno);
			return;
		}

		if (!res)
			break;

		pData = ((const uint8_t *)pData) + res;
		len -= res;

		bytesSent += res;
	}

	if (bytesSent != lenBkup)
		procWrnLog("not all data has been sent");

	mBytesSent += bytesSent;
}

void TcpTransfering::disconnect(int err)
{
	//lock_guard<mutex> lock(mSocketFdMtx); // every caller must lock in advance!

	if (mSocketFd < 0)
	{
		procDbgLog(LOG_LVL, "socket closed already");
		return;
	}

	procDbgLog(LOG_LVL, "closing socket: %d", mSocketFd);
	mErrno = err;
	::close(mSocketFd);
	mSocketFd = -1;
	procDbgLog(LOG_LVL, "closing socket: %d: done", mSocketFd);
}

/* Literature
 * - http://man7.org/linux/man-pages/man2/getsockname.2.html
 * - http://man7.org/linux/man-pages/man2/getpeername.2.html
 * - https://stackoverflow.com/questions/10167540/how-to-get-local-ip-address-and-port-in-unix-socket-programming
 *   - https://beej.us/guide/bgnet/html/multi/getpeernameman.html
 * - http://www.masterraghu.com/subjects/np/introduction/unix_network_programming_v1.3/ch04lev1sec10.html
 * - https://linux.die.net/man/3/inet_ntoa
 *   The inet_ntoa() function converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation.
 *   The string is returned in a statically allocated buffer, which subsequent calls will overwrite.
 */
void TcpTransfering::processInfo(char *pBuf, char *pBufEnd)
{
	{
		lock_guard<mutex> lock(mSocketFdMtx);

		struct sockaddr_in addr;
		socklen_t addrLen;

		memset(&addr, 0, sizeof(addr));

		addrLen = sizeof(addr);
		::getsockname(mSocketFd, (struct sockaddr*)&addr, &addrLen);
		dInfo("%s:%d <--> ", ::inet_ntoa(addr.sin_addr), ::ntohs(addr.sin_port));

		addrLen = sizeof(addr);
		::getpeername(mSocketFd, (struct sockaddr*)&addr, &addrLen);
		dInfo("%s:%d\n", ::inet_ntoa(addr.sin_addr), ::ntohs(addr.sin_port));
	}

	dInfo("Bytes received\t\t%d\n", mBytesReceived);
}
