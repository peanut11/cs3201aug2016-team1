
// Maintained by: Kai Lin
//
// Accessed by:
// - PKB
//
// Uses:
// - TNode

#include "AbstractSyntaxTree.h" 

TNode* AbstractSyntaxTree::createNode(EntityType nodeType)
{
	return new TNode(nodeType);
}

TNode * AbstractSyntaxTree::createNode(EntityType nodeType, int nameIndex)
{
	return new TNode(nodeType, nameIndex);
}
