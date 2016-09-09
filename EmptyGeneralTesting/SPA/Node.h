// Maintained by: Kai Lin & Ngoc Khanh
#include "../SPA/EntityType.h"

#ifndef NODE_H
#define NODE_H

class Node {

public:

	Node(EntityType nodeType);	
	// NodeType: procedure, variable, constant
	Node(EntityType nodeType, int nameIndex);

	EntityType getNodeType();
	int getIndex();
	void setLeft(Node*);
	void setRight(Node*);
	static bool equals(Node*, Node*);
	static void print(Node*);

private:
	EntityType nodeType;
	int index;
	Node *leftChild, *rightChild;
};

#endif