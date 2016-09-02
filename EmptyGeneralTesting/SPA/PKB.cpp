// Maintained by: Kai Lin
//
// Facade of:
// - AbstractSyntaxTree
// - VarTable
// - StmtTable

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"

// create AST Nodes
Node * PKB::createVariableNode(AssignmentTree ast, VAR varNameIndex)
{
	return ast.createNode(VARIABLE, varNameIndex);
}

Node * PKB::createConstanNode(AssignmentTree ast, CONST constantValue)
{
	return ast.createNode(CONSTANT, constantValue);
}

Node * PKB::createNode(AssignmentTree ast, EntityType nodeType)
{
	return ast.createNode(nodeType);
}
