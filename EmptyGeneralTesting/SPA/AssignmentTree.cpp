
// Maintained by: Kai Lin
//
// Accessed by:
// - PKB
//
// Uses:
// - Node

#include "AssignmentTree.h" 

Node* AssignmentTree::createNode(EntityType nodeType)
{
	return new Node(nodeType);
}

Node * AssignmentTree::createNode(EntityType nodeType, int nameIndex)
{
	return new Node(nodeType, nameIndex);
}
