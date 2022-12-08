
#ifndef AOC_TREE_H
#define AOC_TREE_H

#include "Processing.h"

extern uint32_t sizeFile;
extern uint32_t cntCols;
extern uint32_t cntRows;

void lineParse(const char *pLine);
void treesPrint();
uint32_t treeVisible(uint32_t r, uint32_t c);
uint32_t treeScenic(uint32_t r, uint32_t c);

#endif

