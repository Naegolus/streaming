
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "AdventCoding.h"
#include "AocTree.h"

using namespace std;

char line[10000];
uint32_t sumTotal = 0;
uint32_t sum = 0;
uint32_t sizeTotalFree = 70000000;
uint32_t sizeNeededFree = 30000000;
uint32_t sizeFree = 0;
uint32_t sizeNeededLeft = 0;
uint32_t sizeDelete = 0;

void treePrint(TreeNode<DirFile> *pNode, uint32_t depth = 0)
{
	list<TreeNode<DirFile> >::iterator iter;
	char prefix[16];
	uint32_t u = 0;

	//infLog("Depth %d", depth);

	for (u = 0; u < 2 * depth; ++u)
		prefix[u] = ' ';
	prefix[u] = 0;

	infLog("%s %s (%d)", prefix, pNode->t.name.c_str(), pNode->t.size);

	++depth;

	for (iter = pNode->children.begin(); iter != pNode->children.end(); ++iter)
		treePrint(&(*iter), depth);
}

uint32_t sumCalc(TreeNode<DirFile> *pNode)
{
	uint32_t mySize = 0;

	if (pNode->t.size)
	{
		//infLog("File size of %s is %d", pNode->t.name.c_str(), pNode->t.size);
		mySize += pNode->t.size;
	}

	//infLog("Summing %s", pNode->t.name.c_str());

	list<TreeNode<DirFile> >::iterator iter;

	for (iter = pNode->children.begin(); iter != pNode->children.end(); ++iter)
		mySize += sumCalc(&(*iter));

	// END Calculate node size

	if (!sizeNeededLeft and !pNode->t.size and mySize <= 100000)
	{
		infLog("-- Sum of %s is %d", pNode->t.name.c_str(), mySize);
		sum += mySize;
	}

	if (!pNode->t.size and sizeNeededLeft and mySize >= sizeNeededLeft)
	{
#if 1
		wrnLog("-- Sum of %s is %d", pNode->t.name.c_str(), mySize);
#endif
		if (!sizeDelete)
		{
			wrnLog("Init");
			sizeDelete = mySize;
		}

		if (mySize < sizeDelete)
		{
			wrnLog("Lower");
			sizeDelete = mySize;
		}
	}

	return mySize;
}

Success AdventCoding::process()
{
	FILE *pFile = fopen("../input2.txt", "r");

	if (!pFile)
		return procErrLog(-1, "Could not open file");

	while (fgets(line, sizeof(line), pFile))
	{
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;

		//procInfLog("Line: %s", line);

		treeCreate(line);
	}

	fclose(pFile);
	pFile = NULL;

	//treePrint(&tree);

	sumTotal = sumCalc(&tree);
	sizeFree = sizeTotalFree - sumTotal;
	sizeNeededLeft = sizeNeededFree - sizeFree;
	sumCalc(&tree);

	procInfLog("Result1:      %d", sum);
	procInfLog("Total free:   %d", sizeTotalFree);
	procInfLog("Total:        %d", sumTotal);
	procInfLog("Needed free:  %d", sizeNeededFree);
	procInfLog("Free:         %d", sizeFree);
	procInfLog("Needed left:  %d", sizeNeededLeft);
	procInfLog("Delete:       %d", sizeDelete);

	return Positive;
}

