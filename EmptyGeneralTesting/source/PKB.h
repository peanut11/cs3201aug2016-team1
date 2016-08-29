// Maintained by: Kai Lin

#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "../SPA/AbstractSyntaxTree.h"

using namespace std;
typedef short PROC; // index of procedure name in ProcTable
typedef int VAR, CONST; //index of variable name in VarTable and constant value

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	//static int setProcToAST(PROC p, TNode* r);
	//static TNode* getRootAST (PROC p);
	
	// creating AST and returning root node
	static TNode* createAST(string progName);

	// creating AST Nodes
	static TNode* createProcNode(PROC procNameIndex);
	static TNode* createStmtLstNode();
	static TNode* createAssignNode();
	static TNode* createWhileNode();
	static TNode* createVariableNode(VAR varNameIndex);
	static TNode* createConstantNode(CONST constantValue);
	static TNode* createPlusNode();

};
