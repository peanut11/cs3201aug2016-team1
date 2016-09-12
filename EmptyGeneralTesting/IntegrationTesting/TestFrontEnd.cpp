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

			int consts[] = { 0, 2, 5 };
			std::set<Constant> expectedConstSet(consts, consts + 3),
				actualConstSet = pkb->getAllConstantValues();
			Assert::IsTrue(expectedConstSet == actualConstSet);

			EntityType actualType = pkb->getStmtTypeForStmt(1),
				expectedType = ASSIGN;
			Assert::IsTrue(expectedType == actualType);

			actualType = pkb->getStmtTypeForStmt(2);
			expectedType = ASSIGN;
			Assert::IsTrue(expectedType == actualType);

			actualType = pkb->getStmtTypeForStmt(3);
			expectedType = WHILE;
			Assert::IsTrue(expectedType == actualType);

			bool actual = pkb->is(FOLLOWS, 2, 1),
				expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(FOLLOWS, 4, 3);
			expected = false;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(FOLLOWS, 5, 4);
			expected = true;
			Assert::IsTrue(expected == actual);

			VarIndex index = pkb->getVarIndex("i");
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
		}
	};
}