#pragma once
#include "ExprTree.h"

class AssignTree {
public:
	AssignTree();
	AssignTree(VarIndex varIndex, ExprTree &exprTree);
	AssignTree(ProgLine line);
	VarIndex getVar();
	ExprTree getExprTree();

private:
	ExprTree exprTree;
	VarIndex varIndex;
};