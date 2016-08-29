#include "../source/TNode.h"

using namespace std;

#pragma once
class AbstractSyntaxTree
{
public:
	AbstractSyntaxTree(char programName);
	~AbstractSyntaxTree();

	TNode* createNode(TNode::TNodeType nodeType);
	TNode* createNode(TNode::TNodeType nodeType, int nameIndex);
private:
	TNode* mRootNode;
	char mProgramName;
};

