
// Maintained by: Kai Lin
//
// Accessed by:
// - PKB
//
// Uses:
// - TNode

#include "AbstractSyntaxTree.h" 
using namespace std;

AbstractSyntaxTree *AbstractSyntaxTree::_instance;
/*
Singleton
*/
AbstractSyntaxTree *AbstractSyntaxTree::getInstance()
{
	if (!_instance)
		_instance = new AbstractSyntaxTree;
	return _instance;
}

TNode * AbstractSyntaxTree::createRootProgNode(PROC progName)
{
	// assumes this function will only be called once 
	// and doesn't check if root already exists
	mRootNode = new TNode(TNode::nodeTypeProgram);
	mProgramName = progName;
	return mRootNode;
}

TNode* AbstractSyntaxTree::createNode(TNode::TNodeType nodeType)
{
	return new TNode(nodeType);
}

TNode * AbstractSyntaxTree::createNode(TNode::TNodeType nodeType, int nameIndex)
{
	return new TNode(nodeType, nameIndex);
}
