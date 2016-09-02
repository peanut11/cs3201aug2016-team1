
// Maintained by: Kai Lin
//
// Accessed by:
// - PKB
//
// Uses:
// - Node

#include "AssignTree.h" 

Node* AssignTree::createNode(EntityType nodeType)
{
	return new Node(nodeType);
}

Node * AssignTree::createNode(EntityType nodeType, int nameIndex)
{
	return new Node(nodeType, nameIndex);
}
