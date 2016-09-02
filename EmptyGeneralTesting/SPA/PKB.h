// Maintained by: Kai Lin

#pragma once

#include<stdio.h>
#include <iostream>
#include <vector>
#include "AssignmentTree.h"
#include "EntityType.h"

using namespace std;
typedef short PROC; // index of procedure name in ProcTable
typedef int VAR, CONST; //index of variable name in VarTable and constant value

class Node;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	
	// creating AST Nodes
	static Node* createVariableNode(AssignmentTree ast, VAR varNameIndex);
	static Node* createConstanNode(AssignmentTree ast, CONST constantValue);
	static Node* createNode(AssignmentTree ast, EntityType);

};
