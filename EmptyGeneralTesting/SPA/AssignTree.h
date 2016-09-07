#pragma once
#include "ExprTree.h"

class AssignTree
{

public:
	AssignTree();
	AssignTree(VarIndex varIndex, ExprTree exprTree);
	VarIndex getVar();
	ExprTree getExprTree();

private:
	ExprTree exprTree;
	VarIndex varIndex;
};

AssignTree buildAssignTree(ProgLine line, ProgLineNumber lineNum) {

}