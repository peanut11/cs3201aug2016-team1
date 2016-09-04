// Maintained by: Ngoc Khanh
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
	this->nodeType = nodeType;
}

Node::Node(EntityType nodeType, int nameIndex)
{
	this->nodeType = nodeType;
	this->nameIndex = nameIndex;
}

EntityType Node::getNodeType() {
	return nodeType;
}

int Node::getIndex() {
	return index;
}

void Node::setLeft(Node *p) {
	this->leftChild = p;
}

void Node::setRight(Node *p) {
	this->rightChild = p;
}