// Maintained by: Kai Lin

#pragma once

#include<stdio.h>
#include <iostream>
#include <vector>
#include "../SPA/AbstractSyntaxTree.h"
#include "../SPA/EntityType.h"

using namespace std;
typedef short PROC; // index of procedure name in ProcTable
typedef int VAR, CONST; //index of variable name in VarTable and constant value

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	
	// creating AST Nodes
	static TNode* createVariableNode(AbstractSyntaxTree ast, VAR varNameIndex);
	static TNode* createConstantNode(AbstractSyntaxTree ast, CONST constantValue);
	static TNode* createNode(AbstractSyntaxTree ast, EntityType);

};
