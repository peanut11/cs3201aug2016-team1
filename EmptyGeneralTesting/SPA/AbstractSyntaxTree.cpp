
// Maintained by: Kai Lin
//
// Accessed by:
// - PKB
//
// Uses:
// - TNode

#include "AbstractSyntaxTree.h" 


AbstractSyntaxTree::AbstractSyntaxTree(char programName)
{
	mRootNode = new TNode(TNode::nodeTypeProgram);
	mProgramName = programName;
}

AbstractSyntaxTree::~AbstractSyntaxTree()
{
}

TNode* AbstractSyntaxTree::createNode(TNode::TNodeType nodeType)
{
	return new TNode(nodeType);
}

TNode * AbstractSyntaxTree::createNode(TNode::TNodeType nodeType, int nameIndex)
{
	return new TNode(nodeType, nameIndex);
}
