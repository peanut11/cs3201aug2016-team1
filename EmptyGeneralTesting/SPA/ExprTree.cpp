#include "ExprTree.h"
#include "PKB.h"

const std::string DUMMY_TOKEN = "~";

ExprTree::ExprTree() {
}

ExprTree::ExprTree(Node* root) {
	this->root = root;
}

Node* ExprTree::getRoot() {
	return root;
}

ExprTree::ExprTree(ProgLine line) {
	line.push_back(DUMMY_TOKEN);
	PKB *pkb = PKB::getInstance();
	std::stack<Node*> nodeStack;
	std::stack<std::string> opStack;

	for (size_t i = 0; i < line.size(); i++) {
		if (line[i].size() == 0) continue;

		if ((line[i] == "+" || line[i] == "-" || line[i] == DUMMY_TOKEN)
			&& !opStack.empty()) {
			popOnce(nodeStack, opStack);
		}

		if (!isalnum(line[i].at(0))) {
			opStack.push(line[i]);
		}
		else if (isalpha(line[i].at(0))) {
			nodeStack.push(new Node(VARIABLE, pkb->getVarIndex(line[i])));
		}
		else {
			nodeStack.push(new Node(CONSTANT, pkb->getVarIndex(line[i])));
		}
	}

	root = nodeStack.top();
}

void ExprTree::popOnce(std::stack<Node*> &nodeStack, std::stack<std::string> &opStack) {
	std::string oper = opStack.top();
	opStack.pop();
	Node* rightOperand = nodeStack.top();
	nodeStack.pop();
	Node* leftOperand = nodeStack.top();
	nodeStack.pop();

	Node* operatorNode;
	if (oper == "+") {
		operatorNode = new Node(PLUS);
	}
	else if (oper == "-") {
		operatorNode = new Node(MINUS);
	}
	else if (oper == "*") {
		operatorNode = new Node(TIMES);
	}

	operatorNode->setLeft(leftOperand);
	operatorNode->setRight(rightOperand);
	nodeStack.push(operatorNode);
}
