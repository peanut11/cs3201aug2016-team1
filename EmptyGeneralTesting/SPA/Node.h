// Maintained by: Kai Lin & Ngoc Khanh
#include "../SPA/EntityType.h"

#ifndef NODE_H
#define NODE_H

#include "Types.h"

class Node {

public:

	Node(EntityType);	
	// NodeType: procedure, variable, constant
	Node(EntityType, StringToken);

	EntityType getNodeType();
	StringToken getValue();
	void setLeft(Node*);
	void setRight(Node*);
	static bool equals(Node*, Node*);
	static StringToken toString(Node*);

private:
	EntityType nodeType;
	StringToken value;
	Node *leftChild, *rightChild;
};

#endif