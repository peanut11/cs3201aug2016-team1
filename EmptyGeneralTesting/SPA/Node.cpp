// Maintained by: Kai Lin & Ngoc Khanh
//
// Constructed by:
// - ProgramConvertor
// - PKB
//
// Used by:
// - DesignExtractor

#include "Node.h"

Node::Node(EntityType nodeType)
{
	//assumes Entity type is valid. ie only assign related
	mNodeType = nodeType;
}


Node::Node(EntityType nodeType, int nameIndex)
{
	mNodeType = nodeType;
	mNameIndex = nameIndex;
}

EntityType Node::geNodeType()
{
	return mNodeType;
}

int Node::getNameIndex()
{
	return mNameIndex;
}
