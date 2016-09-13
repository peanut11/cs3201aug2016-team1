#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Frontend.h"
#include "../SPA/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {		
	TEST_CLASS(IntegrationTest) {
	public:
		
		TEST_METHOD(TestPKB_Basic) {
			Frontend frontend = Frontend();
			frontend.parse(std::string("Source1_Basic.txt"));

			PKB* pkb = PKB::getInstance();

			VarIndex index = pkb->getVarIndex("i");
			VarName actualName = pkb->getVarName(index),
				expectedName = "i";
			Assert::IsTrue(expectedName == actualName);

			bool actual = pkb->is(FOLLOWS, 1, 2),
				expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(FOLLOWS, 3, 4);
			expected = false;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(FOLLOWS, 4, 5);
			expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(MODIFIES, 2, index);
			expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(MODIFIES, 5, index);
			expected = false;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(USES, 4, index);
			expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(USES, 2, index);
			expected = false;
			Assert::IsTrue(expected == actual);

			Assert::IsTrue(pkb->isAssignHasExpr(2, "5"));
			Assert::IsFalse(pkb->isAssignHasExpr(2, "x"));

			Assert::IsTrue(pkb->isAssignHasExpr(4, "z   +  x  + i "));
			Assert::IsFalse(pkb->isAssignHasExpr(4, "z+x"));

			Assert::IsTrue(pkb->isAssignHasSubexpr(4, "  z  + x +i"));
			Assert::IsTrue(pkb->isAssignHasSubexpr(4, " z + x   "));
			Assert::IsTrue(pkb->isAssignHasSubexpr(4, " i "));
			Assert::IsFalse(pkb->isAssignHasSubexpr(4, "x + i"));

			Assert::IsTrue(pkb->isVarExist("z"));
			Assert::IsFalse(pkb->isVarExist("t"));

			EntityType actualType = pkb->getStmtTypeForStmt(1),
				expectedType = ASSIGN;
			Assert::IsTrue(expectedType == actualType);

			actualType = pkb->getStmtTypeForStmt(2);
			expectedType = ASSIGN;
			Assert::IsTrue(expectedType == actualType);

			actualType = pkb->getStmtTypeForStmt(3);
			expectedType = WHILE;
			Assert::IsTrue(expectedType == actualType);

			Assert::IsTrue(pkb->getStmtTableSize() == 5);

			int consts[] = { 0, 2, 5 };
			std::set<Constant> expectedConstSet(consts, consts + 3),
				actualConstSet = pkb->getAllConstantValues();
			Assert::IsTrue(expectedConstSet == actualConstSet);
		}

		TEST_METHOD(TestPKB_LongProcedure) {

		}
	};
}