// Maintained by: Ngoc Khanh
//
// Accessed by:
// - PKB
//
// Uses:
// - Node

#include "AssignTree.h"
#include "PKB.h"
#include <iostream>
#include <cassert>

AssignTree::AssignTree() {
}

AssignTree::AssignTree(VarIndex varIndex, ExprTree &exprTree) {
	this->varIndex = varIndex;
	this->exprTree = exprTree;
}

AssignTree::AssignTree(ProgLine line) {
	assert(line.size() > 2);
	assert(line[1] == "=");

	if (line.back() == ";") {
		line.pop_back();
	}

	PKB *pkb = PKB::getInstance();
	varIndex = pkb->getVarIndex(line[0]);
	line.erase(line.begin(), line.begin() + 2);
	exprTree = ExprTree(line);
}

bool AssignTree::equals(AssignTree tree1, AssignTree tree2) {
	return tree1.varIndex == tree2.varIndex &&
		ExprTree::equals(tree1.exprTree, tree2.exprTree);
}

void AssignTree::print(AssignTree tree) {
	std::cout << tree.varIndex << " = ";
	ExprTree::print(tree.exprTree);
	std::cout << "\n";
}

VarIndex AssignTree::getVar() {
	return varIndex;
}

ExprTree AssignTree::getExprTree() {
	return exprTree;
}