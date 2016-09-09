#pragma once
#include "Node.h"
#include "Types.h"
#include <stack>

class ExprTree {
public:
	ExprTree();
	ExprTree(Node*);
	ExprTree(ProgLine);
	Node* getRoot();
	static bool equals(ExprTree, ExprTree);
	static void print(ExprTree);

private:
	Node* root;
	void popOnce(std::stack<Node*>&, std::stack<std::string>&);
};