#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestIsAssignHasExpr) {
public:

	TEST_METHOD(TestIsAssignHasExpr_True_Basic) {
		PKB* pkb = PKB::getInstance();
		Assert::IsTrue(pkb->isAssignHasExpr(1, "3 + 4"));
	}

	TEST_METHOD(TestIsValidProgram_False_Basic) {
		PKB* pkb = PKB::getInstance();
	}

	};
}
