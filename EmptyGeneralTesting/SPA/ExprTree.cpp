#include "ExprTree.h"

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
			nodeStack.push(new Node(VARIABLE, line[i]));
		}
		else {
			nodeStack.push(new Node(CONSTANT, line[i]));
		}
	}

	root = nodeStack.top();
	root->buildSize();
}

bool ExprTree::equals(ExprTree tree1, ExprTree tree2) {
	return Node::equals(tree1.root, tree2.root);
}

bool ExprTree::contains(ExprTree subTree) {
	return Node::isSubTree(root, subTree.root);
}

StringToken ExprTree::toString(ExprTree tree) {
	return Node::toString(tree.root);
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
