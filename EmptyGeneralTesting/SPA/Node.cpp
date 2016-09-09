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

void Node::print(Node* node) {
	if (node != nullptr) {
		std::cout << " (";
		if (node->nodeType == PLUS) {
			std::cout << " + ";
		}
		else if (node->nodeType == VARIABLE) {
			std::cout << "Var " << node->index;
		}
		else if (node->nodeType == CONSTANT) {
			std::cout << "Const " << node->index;
		}
		std::cout << ") ";
	}
	else {
		std::cout << "!!";
	}
}

Node::Node(EntityType nodeType, int nameIndex)
{
	this->nodeType = nodeType;
	this->index = index;
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

bool Node::equals(Node* node1, Node* node2) {
	if (node1 == nullptr) {
		return node2 == nullptr;
	}

	if (node2 == nullptr) {
		return false;
	}

	return (node1->nodeType) == (node2->nodeType) &&
		(node1->index) == (node2->index) &&
		Node::equals(node1->leftChild, node2->leftChild) &&
		Node::equals(node1->rightChild, node2->rightChild);
}
