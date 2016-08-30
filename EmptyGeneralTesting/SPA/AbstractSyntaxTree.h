#include <string>
#include "../source/TNode.h"
typedef short PROC;

using namespace std;

#pragma once
class AbstractSyntaxTree
{

public:
	static AbstractSyntaxTree *getInstance();

	TNode* createRootProgNode(string progName);
	TNode* createNode(TNode::TNodeType nodeType);
	TNode* createNode(TNode::TNodeType nodeType, int nameIndex);
private:
	static AbstractSyntaxTree *_instance;
	TNode* mRootNode;
	string mProgramName;
};
