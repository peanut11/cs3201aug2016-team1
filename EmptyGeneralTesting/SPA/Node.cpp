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
	leftChild = nullptr;
	rightChild = nullptr;
}

Node::Node(EntityType nodeType, StringToken value) {
	this->nodeType = nodeType;
	this->value = value;
	leftChild = nullptr;
	rightChild = nullptr;
}

EntityType Node::getNodeType() {
	return nodeType;
}

StringToken Node::getValue() {
	return value;
}

int Node::size() {
	return nCount;
}

void Node::setLeft(Node *p) {
	this->leftChild = p;
}

void Node::setRight(Node *p) {
	this->rightChild = p;
}

void Node::buildSize() {
	if (leftChild == nullptr) {
		nCount = 1;
	}
	else {
		leftChild->buildSize();
		rightChild->buildSize();
		nCount = leftChild->size() + 1 + rightChild->size();
	}
}

bool Node::equals(Node* node1, Node* node2) {
	if (node1 == nullptr) {
		return node2 == nullptr;
	}

	if (node2 == nullptr) {
		return false;
	}

	return (node1->nodeType) == (node2->nodeType) &&
		(node1->value) == (node2->value) &&
		Node::equals(node1->leftChild, node2->leftChild) &&
		Node::equals(node1->rightChild, node2->rightChild);
}

bool Node::isSubTree(Node* tree, Node* subTree) {
	if (tree == nullptr || subTree == nullptr) {
		return false;
	}

	int treeSize = tree->nCount,
		subTreeSize = subTree->nCount;

	if (treeSize < subTreeSize) {
		return false;
	}

	if (treeSize == subTreeSize) {
		return Node::equals(tree, subTree);
	}

	return Node::isSubTree(tree->leftChild, subTree) ||
		Node::isSubTree(tree->rightChild, subTree);
}

StringToken Node::toString(Node* p) {
	if (p == nullptr) return "";
	return Node::toString(p->leftChild) + " " + p->value + " " + Node::toString(p->rightChild);
}