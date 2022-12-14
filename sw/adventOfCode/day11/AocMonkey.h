
#ifndef AOC_MONKEY_H
#define AOC_MONKEY_H

#include <jsoncpp/json/json.h>

extern Json::Value notes;

void lineParse(char *pLine);
void superModCalc();
void numRoundsExec(uint32_t numRounds, bool noWorry = true);
void monkeyItemsPrint();
uint32_t monkeyCntThrownPrint();

#endif

