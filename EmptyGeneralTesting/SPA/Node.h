// Maintained by: Kai Lin & Ngoc Khanh
#include "../SPA/EntityType.h"

#ifndef NODE_H
#define NODE_H

class Node
{

public:

	Node(EntityType nodeType);	
	// NodeType: procedure, variable, constant
	Node(EntityType nodeType, int nameIndex);

	EntityType geNodeType();
	int getNameIndex();
private:
	EntityType mNodeType;
	int mNameIndex;
};

#endif