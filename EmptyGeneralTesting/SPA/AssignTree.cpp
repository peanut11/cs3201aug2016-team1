// Maintained by: Ngoc Khanh
//
// Accessed by:
// - PKB
//
// Uses:
// - Node

#include "AssignTree.h"
#include <cassert>

AssignTree::AssignTree() {
}

AssignTree::AssignTree(VarIndex varIndex, ExprTree exprTree) {
	this->varIndex = varIndex;
	this->exprTree = exprTree;
}

VarIndex AssignTree::getVar() {
	return varIndex;
}

ExprTree AssignTree::getExprTree() {
	return exprTree;
}

AssignTree buildAssignTree(ProgLine line) {
	assert(line.size() > 2);
	assert(line[1] == "=");

	if (line.back() == ";") {
		line.pop_back();
	}

	PKB *pkb = PKB::getInstance();
	VarIndex varIndex = pkb->getVarIndex(line[0]);
	line.erase(line.begin(), line.begin() + 2);
	ExprTree exprTree = buildExprTree(line);
	return AssignTree(varIndex, exprTree);
}