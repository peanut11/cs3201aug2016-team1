#pragma once
#include "ExprTree.h"

class AssignTree {
public:
	AssignTree();
	AssignTree(VarIndex, ExprTree&);
	AssignTree(ProgLine);
	VarIndex getVar();
	ExprTree getExprTree();
	static bool equals(AssignTree, AssignTree);
	static void print(AssignTree);

private:
	ExprTree exprTree;
	VarIndex varIndex;
};