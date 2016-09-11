#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/ProgramConverter.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestIsAssignHasExpr) {
public:

	TEST_METHOD(TestIsAssignHasExpr_True_Basic) {
		PKB* pkb = PKB::getInstance();

		StringToken arr[] = { "x", "=", "u", "+", "9" };
		ProgLine vec(arr, arr + sizeof arr / sizeof arr[0]);

/*		AssignTree tree = AssignTree(vec);
		Logger::WriteMessage((AssignTree::toString(tree)).c_str()); 

		Logger::WriteMessage((ExprTree::toString(tree.getExprTree())).c_str()); 

		pkb->putAssignForStmt(2, tree); */

		ProgramConverter converter = ProgramConverter();

		converter.updateStmtInStmtTable(vec, 1);

		for (size_t i = 0; i < 10; i++) {
			AssignTree tt = pkb->getAssign(0);
			Logger::WriteMessage((AssignTree::toString(tt)).c_str());
			Logger::WriteMessage("\n-----\n");
		}

		Assert::IsTrue(pkb->isAssignHasExpr(0, "u + 9"));
	}

	TEST_METHOD(TestIsValidProgram_False_Basic) {
		PKB* pkb = PKB::getInstance();
	}

	};
}
