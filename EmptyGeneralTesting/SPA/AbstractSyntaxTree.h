#include "../source/TNode.h"

#pragma once
class AbstractSyntaxTree
{

public:

	TNode* createNode(EntityType nodeType);
	TNode* createNode(EntityType nodeType, int nameIndex);
private:
	
};

