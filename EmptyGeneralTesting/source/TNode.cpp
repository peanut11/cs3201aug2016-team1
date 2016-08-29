// Maintained by: Kai Lin & Ngoc Khanh
//
// Constructed by:
// - ProgramConvertor
// - PKB
//
// Used by:
// - DesignExtractor

#include "TNode.h"

TNode::TNode(TNodeType nodeType)
{
	mNodeType = nodeType;
}


TNode::TNode(TNodeType nodeType, int nameIndex)
{
	mNodeType = nodeType;
	mNameIndex = nameIndex;
}

TNode::TNodeType TNode::getNodeType()
{
	return mNodeType;
}

int TNode::getNameIndex()
{
	return mNameIndex;
}
