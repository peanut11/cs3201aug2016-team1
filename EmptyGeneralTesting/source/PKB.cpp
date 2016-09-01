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

// create AST Nodes
TNode * PKB::createVariableNode(AbstractSyntaxTree ast, VAR varNameIndex)
{
	return ast.createNode(VARIABLE, varNameIndex);
}

TNode * PKB::createConstantNode(AbstractSyntaxTree ast, CONST constantValue)
{
	return ast.createNode(CONSTANT, constantValue);
}

TNode * PKB::createNode(AbstractSyntaxTree ast, EntityType nodeType)
{
	return ast.createNode(nodeType);
}
