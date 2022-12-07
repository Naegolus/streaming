
#include "AocTree.h"
#include "Processing.h"

using namespace std;

list<string> pwd;
TreeNode<DirFile> tree;
TreeNode<DirFile> *pCurrent = &tree;

static uint32_t state = 0;

static TreeNode<DirFile> *currentChildFind(const char *pName)
{
	list<TreeNode<DirFile> >::iterator iter;

	for (iter = pCurrent->children.begin(); iter != pCurrent->children.end(); ++iter)
	{
		if (iter->t.name == pName)
			return &(*iter);
	}

	return NULL;
}

static TreeNode<DirFile> *nodeAdd(const char *name, uint32_t size = 0)
{
	TreeNode<DirFile> *pNode;

	pNode = currentChildFind(name);
	if (pNode)
		return pNode;

	TreeNode<DirFile> df;

	df.t.name = name;
	df.t.size = size;
	df.pParent = pCurrent;

	//wrnLog("Creating node %s in %s, size = %d", name, pCurrent->t.name.c_str(), size);
	pCurrent->children.push_back(df);

	return &pCurrent->children.back();;
}

void treeCreate(const char *pLine)
{
	TreeNode<DirFile> *pDir;
	const char *pRef;
	uint32_t len, size;
	char name[64];
	int res;

	if (state == 1 and pLine[0] == '$')
	{
		//infLog("List Mode: Done");
		state = 0;
	}

	//infLog("Line: %s", pLine);

	switch (state)
	{
	case 0:
#if 0
		if (!strcmp(pLine, "$ cd /"))
		{
			pCurrent->t.name = "/";
			pCurrent->t.size = 0;
			break;
		}
#endif
		if (!strcmp(pLine, "$ cd .."))
		{
			infLog("Leaving %s", pCurrent->t.name.c_str());

			if (pCurrent == &tree)
				break;

			pCurrent = pCurrent->pParent;
			pwd.pop_back();
			//infLog("Entered %s", pCurrent->t.name.c_str());

			break;
		}

		pRef = "$ cd ";
		len = strlen(pRef);

		if (!strncmp(pLine, pRef, len))
		{
			pLine += len;
			pDir = nodeAdd(pLine);

			infLog("Entering %s from %s", pLine, pCurrent->t.name.c_str());
			pwd.push_back(pLine);

			if (!pDir)
				errLog(-1, "Could not find directory %s", pLine);
			pCurrent = pDir;
			//infLog("Entered %s", pCurrent->t.name.c_str());

			break;
		}

		if (!strcmp(pLine, "$ ls"))
		{
			//infLog("List Mode");
			state = 1;
			break;
		}

		break;
	case 1:

		pRef = "dir ";
		len = strlen(pRef);

		if (!strncmp(pLine, pRef, len))
		{
			pLine += len;
			nodeAdd(pLine, 0);
			break;
		}

		res = sscanf(pLine, "%u %s", &size, name);
		if (res != 2)
		{
			errLog(-1, "Could not parse line");
			break;
		}

		nodeAdd(name, size);

		break;
	default:
		break;
	}
}

