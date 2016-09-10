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
	int size();
	void setLeft(Node*);
	void setRight(Node*);
	static bool isSubTree(Node*, Node*);
	static bool equals(Node*, Node*);
	static StringToken toString(Node*);
	void buildSize();

private:
	EntityType nodeType;
	StringToken value;
	Node *leftChild, *rightChild;
	int nCount;
};

#endif