#include "../source/TNode.h"

#pragma once
class AbstractSyntaxTree
{

public:

	TNode* createNode(TNode::TNodeType nodeType);
	TNode* createNode(TNode::TNodeType nodeType, int nameIndex);
private:
	
};

