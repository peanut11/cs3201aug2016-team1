// Maintained by: Ngoc Khanh
//
// Constructed by:
// - ProgramConvertor
// - PKB
//
// Used by:
// - DesignExtractor

#include "Node.h"
#include <iostream>

Node::Node(EntityType nodeType)
{
	//assumes Entity type is valid. ie only assign related
	this->nodeType = nodeType;
}

Node::Node(EntityType nodeType, StringToken value) {
	this->nodeType = nodeType;
	this->value = value;
	this->leftChild = nullptr;
	this->rightChild = nullptr;
}

EntityType Node::getNodeType() {
	return nodeType;
}

StringToken Node::getValue() {
	return value;
}

void Node::setLeft(Node *p) {
	this->leftChild = p;
}

void Node::setRight(Node *p) {
	this->rightChild = p;
}

bool Node::equals(Node* node1, Node* node2) {
	if (node1 == nullptr) {
		return node2 == nullptr;
	}

	else if (node2 == nullptr) {
		return false;
	}
	else {
		return (node1->nodeType) == (node2->nodeType) &&
			(node1->value) == (node2->value) &&
			Node::equals(node1->leftChild, node2->leftChild) &&
			Node::equals(node1->rightChild, node2->rightChild);
	}
}

StringToken Node::toString(Node* p) {
	if (p == nullptr) return "";
	return Node::toString(p->leftChild) + " " + p->value + " " + Node::toString(p->rightChild);
}