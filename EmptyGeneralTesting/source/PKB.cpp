// Maintained by: Kai Lin
//
// Facade of:
// - AbstractSyntaxTree
// - VarTable
// - StmtTable

#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"

//int PKB::setProcTo(AbstractSyntaxTree::getInstance)(PROC p, TNode* r) {
//	return NULL;
//}

//TNode* PKB::getRoot(AbstractSyntaxTree::getInstance) (PROC p){
//	return NULL;
//}

TNode * PKB::createVariableNode(VAR varNameIndex)
{
	return AbstractSyntaxTree::getInstance()->createNode(TNode::nodeTypeVariable, varNameIndex);
}

TNode * PKB::createConstantNode(CONST constantValue)
{
	return AbstractSyntaxTree::getInstance()->createNode(TNode::nodeTypeVariable, constantValue);
}

TNode * PKB::createPlusNode()
{
	return AbstractSyntaxTree::getInstance()->createNode(TNode::nodeTypePlus);
}
