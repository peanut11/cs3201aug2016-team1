// Maintained by: Kai Lin & Ngoc Khanh
#include "../SPA/EntityType.h"

#ifndef TNODE_H
#define TNODE_H

class TNode
{

public:

	TNode(EntityType nodeType);	
	// NodeType: procedure, variable, constant
	TNode(EntityType nodeType, int nameIndex);

	EntityType getNodeType();
	int getNameIndex();
private:
	EntityType mNodeType;
	int mNameIndex;
};

#endif