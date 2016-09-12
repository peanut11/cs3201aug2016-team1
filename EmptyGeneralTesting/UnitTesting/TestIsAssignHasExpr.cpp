#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestIsAssignHasExpr) {
public:

	TEST_METHOD(TestIsAssignHasExpr_True_Basic) {
		PKB* pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "x", "=", "u", "+", "9" };
		ProgLine vec(arr, arr + sizeof arr / sizeof arr[0]);

		AssignTree tree = AssignTree(vec);
		Logger::WriteMessage((AssignTree::toString(tree)).c_str()); 

		Logger::WriteMessage((ExprTree::toString(tree.getExprTree())).c_str()); 

		pkb->putAssignForStmt(0, tree);

		Assert::IsTrue(pkb->isAssignHasExpr(0, "u+9"));
		Assert::IsTrue(pkb->isAssignHasExpr(0, "u + 9"));
		Assert::IsTrue(pkb->isAssignHasExpr(0, "u +   9"));
		Assert::IsTrue(pkb->isAssignHasExpr(0, "   u     +     9"));
	}

	TEST_METHOD(TestIsAssignHasExpr_False_Basic) {
		PKB* pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "x", "=", "u", "+", "9" };
		ProgLine vec(arr, arr + sizeof arr / sizeof arr[0]);

		AssignTree tree = AssignTree(vec);
		Logger::WriteMessage((AssignTree::toString(tree)).c_str());

		Logger::WriteMessage((ExprTree::toString(tree.getExprTree())).c_str());

		pkb->putAssignForStmt(0, tree);

		Assert::IsFalse(pkb->isAssignHasExpr(0, "u + 10"));
		Assert::IsFalse(pkb->isAssignHasExpr(0, "9 + u"));
		Assert::IsFalse(pkb->isAssignHasExpr(0, "u+v"));
	}

	TEST_METHOD(TestIsAssignHasSubexpr_True_Basic) {
		PKB* pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "a", "=", "u", "+", "10", "+", "5"};

		ProgLine vec(arr, arr + sizeof arr / sizeof arr[0]);

		AssignTree tree = AssignTree(vec);
		Logger::WriteMessage((AssignTree::toString(tree)).c_str());

		Logger::WriteMessage((ExprTree::toString(tree.getExprTree())).c_str());

		pkb->putAssignForStmt(0, tree);

		Assert::IsTrue(pkb->isAssignHasSubexpr(0, "u+10+5"));
		Assert::IsTrue(pkb->isAssignHasSubexpr(0, "u  +  10"));
	}

	TEST_METHOD(TestIsAssignHasSubexpr_True_LongExpr) {
		PKB* pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "abc", "=", "longname", "+", "longername", "+", "1234567", "+",
			"what", "+", "8910", "+", "many", "+", "other", "+", "things"};

		ProgLine vec(arr, arr + sizeof arr / sizeof arr[0]);

		AssignTree tree = AssignTree(vec);
		Logger::WriteMessage((AssignTree::toString(tree)).c_str());

		Logger::WriteMessage((ExprTree::toString(tree.getExprTree())).c_str());

		pkb->putAssignForStmt(0, tree);

		Assert::IsTrue(pkb->isAssignHasSubexpr(0, "longname + longername  +  1234567  + what  + 8910"));
		Assert::IsTrue(pkb->isAssignHasSubexpr(0, "  longname +  longername  +  1234567  "));
		Assert::IsTrue(pkb->isAssignHasSubexpr(0, "  other"));
		Assert::IsTrue(pkb->isAssignHasSubexpr(0, "  othe  r")); //this should not happens
		Assert::IsTrue(pkb->isAssignHasSubexpr(0, " 1234567 "));
	}

	TEST_METHOD(TestIsAssignHasSubexpr_False_LongExpr) {
		PKB* pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "abc", "=", "longname", "+", "longername", "+", "1234567", "+",
			"what", "+", "8910", "+", "many", "+", "other", "+", "things" };

		ProgLine vec(arr, arr + sizeof arr / sizeof arr[0]);

		AssignTree tree = AssignTree(vec);
		Logger::WriteMessage((AssignTree::toString(tree)).c_str());

		Logger::WriteMessage((ExprTree::toString(tree.getExprTree())).c_str());

		pkb->putAssignForStmt(0, tree);

		Assert::IsFalse(pkb->isAssignHasSubexpr(0, " 8910  +   many  +   other + things"));
		Assert::IsFalse(pkb->isAssignHasSubexpr(0, "longername  +  1234567  "));
		Assert::IsFalse(pkb->isAssignHasSubexpr(0, "  othe"));
	}

	};
}
