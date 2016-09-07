#pragma once
#include "Node.h"
#include "PKB.h"
#include <stack>

class ExprTree {
public:
	ExprTree();
	ExprTree(Node* root);
	Node* getRoot();

private:
	Node* root;
};

ExprTree buildExprTree(ProgLine line);
