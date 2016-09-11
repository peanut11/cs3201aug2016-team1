// Maintained by: Ngoc Khanh
//
// Accessed by:
// - PKB
//
// Uses:
// - Node

#include "AssignTree.h"
#include <iostream>
#include <cassert>

AssignTree::AssignTree() {
}

AssignTree::AssignTree(StringToken var, ExprTree exprTree) {
	this->var = var;
	this->exprTree = exprTree;
}

AssignTree::AssignTree(ProgLine line) {
	assert(line.size() > 2);
	assert(line[1] == "=");

	if (line.back() == ";") {
		line.pop_back();
	}

	var = line[0];
	line.erase(line.begin(), line.begin() + 2);
	exprTree = ExprTree(line);
}

bool AssignTree::equals(AssignTree tree1, AssignTree tree2) {
	return tree1.var == tree2.var &&
		ExprTree::equals(tree1.exprTree, tree2.exprTree);
}

StringToken AssignTree::toString(AssignTree tree) {
	return tree.var + "=" + ExprTree::toString(tree.exprTree);
}

StringToken AssignTree::getVar() {
	return var;
}

ExprTree AssignTree::getExprTree() {
	return exprTree;
}