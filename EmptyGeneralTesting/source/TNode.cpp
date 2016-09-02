// Maintained by: Kai Lin & Ngoc Khanh
//
// Constructed by:
// - ProgramConvertor
// - PKB
//
// Used by:
// - DesignExtractor

#include "TNode.h"

TNode::TNode(EntityType nodeType)
{
	//assumes Entity type is valid. ie only assign related
	mNodeType = nodeType;
}


TNode::TNode(EntityType nodeType, int nameIndex)
{
	mNodeType = nodeType;
	mNameIndex = nameIndex;
}

EntityType TNode::getNodeType()
{
	return mNodeType;
}

int TNode::getNameIndex()
{
	return mNameIndex;
}
