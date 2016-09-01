
// Maintained by: Kai Lin
//
// Accessed by:
// - PKB
//
// Uses:
// - TNode

#include "AbstractSyntaxTree.h" 

TNode* AbstractSyntaxTree::createNode(TNode::TNodeType nodeType)
{
	return new TNode(nodeType);
}

TNode * AbstractSyntaxTree::createNode(TNode::TNodeType nodeType, int nameIndex)
{
	return new TNode(nodeType, nameIndex);
}
