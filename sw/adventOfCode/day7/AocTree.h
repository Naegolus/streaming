
#ifndef AOC_TREE_H
#define AOC_TREE_H

#include <string>
#include <list>

struct DirFile
{
	std::string name;
	uint32_t size;
};

template<typename T>
struct TreeNode
{
	T t;
	std::list<TreeNode> children;
	TreeNode *pParent;
};

extern std::list<std::string> pwd;
extern TreeNode<DirFile> tree;
extern TreeNode<DirFile> *pCurrent;

void treeCreate(const char *pLine);

#endif

