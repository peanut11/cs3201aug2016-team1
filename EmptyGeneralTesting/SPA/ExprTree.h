#pragma once
#include "Node.h"
#include "Types.h"
#include <stack>

class ExprTree {
public:
	ExprTree();
	ExprTree(Node* root);
	ExprTree(ProgLine line);
	Node* getRoot();

private:
	Node* root;
	void popOnce(std::stack<Node*> &nodeStack, std::stack<std::string> &opStack);
};