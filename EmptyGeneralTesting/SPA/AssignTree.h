#pragma once
#include "ExprTree.h"

class AssignTree {
public:
	AssignTree();
	AssignTree(StringToken, ExprTree);
	AssignTree(ProgLine);
	StringToken getVar();
	ExprTree getExprTree();
	static bool equals(AssignTree, AssignTree);
	static StringToken toString(AssignTree);

private:
	ExprTree exprTree;
	StringToken var;
};