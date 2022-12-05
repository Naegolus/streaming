
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include "AdventCoding.h"

using namespace std;
using namespace CryptoPP;

char line[10000];
string prefix;
uint32_t nonce = 0;
uint32_t cntPrint = 0;

void hexDump(const void *pData, size_t len, size_t colWidth = 16)
{
	const char *pByte = (const char *)pData;
	uint32_t addressAbs = 0;
	char bufLine[256];
	char *pBufEnd = bufLine + sizeof(bufLine);
	char *pBuf = bufLine;
	const char *pLine = pByte;
	uint8_t lenPrinted;
	uint8_t numBytesPerLine = colWidth;
	size_t i = 0;

	while (len)
	{
		pBuf = bufLine;
		*pBuf = 0;
		pLine = pByte;
		lenPrinted = 0;

		dInfo("%08x", addressAbs);

		for (i = 0; i < numBytesPerLine; ++i)
		{
			if (!(i & 7))
				dInfo(" ");

			if (!len)
			{
				dInfo("   ");
				continue;
			}

			dInfo(" %02x", (uint8_t)*pByte);

			++pByte;
			--len;
			++lenPrinted;
		}

		dInfo("  |");

		for (i = 0; i < lenPrinted; ++i, ++pLine, ++pBuf)
		{
			char c = *pLine;

			if (c < 32 or c > 126)
			{
				*pBuf = '.';
				continue;
			}

			*pBuf = c;
		}

		dInfo("|");

		cout << bufLine << endl;

		addressAbs += lenPrinted;
	}
}

/* member functions */
Success AdventCoding::initialize()
{
	FILE *pFile = fopen("../input.txt", "r");

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		prefix = line;
		procInfLog("prefix: %s", prefix.c_str());
	}

	fclose(pFile);
	pFile = NULL;

	nonce = 0;

	return Positive;
}

uint32_t nonceMax = 10000000;

Success AdventCoding::process()
{
	uint32_t numBurst = 64;
	size_t u;
	string msg, digest;
	Weak::MD5 hash;
	char dBuf[16];

	++cntPrint;
	if (cntPrint > 1000)
	{
		char buf[128];
		cout << "\r";
		progressStr(buf, buf + sizeof(buf), nonce, nonceMax);
		cout << buf;
		cout.flush();
		cntPrint = 0;
	}

	if (nonce > nonceMax)
	{
		cout << endl;
		return procErrLog(-1, "Could not find MD5 hash");
	}

	for (u = 0; u < numBurst; ++u, ++nonce)
	{
		//procInfLog("Nonce: %d", nonce);
		msg = prefix + to_string(nonce);
		//procInfLog("Msg: %s", msg.c_str());

		hash.Restart();
		hash.Update((const byte*)&msg[0], msg.size());
		digest.resize(hash.DigestSize());
		hash.Final((byte*)&digest[0]);

		//procInfLog("Digest:");
		//hexDump(digest.c_str(), digest.size());

		snprintf(dBuf, sizeof(dBuf), "%02x%02x%02x",
								(uint8_t)digest[0],
								(uint8_t)digest[1],
								(uint8_t)digest[2]);

		if (strncmp(dBuf, "000000", 6))
			continue;

		cout << "\r";
#if 1
		procInfLog("Nonce: %d", nonce);

		procInfLog("Digest:");
		hexDump(digest.c_str(), digest.size());
#endif
		return Positive;
	}

	return Pending;
}

