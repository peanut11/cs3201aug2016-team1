#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Frontend.h"
#include "../SPA/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {		
	TEST_CLASS(IntegrationTest) {
	public:
		
		TEST_METHOD(TestPKB_AllBasicMethods) {
			PKB* pkb = PKB::getInstance();
			pkb->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("procedure_Second.txt"));

			pkb = PKB::getInstance();

			VarIndex index = pkb->getVarIndex("i");
			VarName actualName = pkb->getVarName(index),
				expectedName = "i";
			Assert::IsTrue(expectedName == actualName);

			bool actual = pkb->is(FOLLOWS, 1, 2),
				expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(FOLLOWS, 1, 3);
			expected = false;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(FOLLOWS, 4, 5);
			expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(FOLLOWS_STAR, 1, 3);
			expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(PARENT, 3, 5);
			expected = true;
			Assert::IsTrue(expected == actual);

			actual = pkb->is(PARENT_STAR, 3, 5);
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

			StmtNumber stmts[] = { 1, 2, 3, 4, 5 };
			std::set<StmtNumber> expectedStmtSet(stmts, stmts + 5),
				actualStmtSet = pkb->getAllStmts();
			Assert::IsTrue(expectedStmtSet == actualStmtSet);

			VarName varNames[] = { "x", "i", "y", "z" };
			std::set<VarName> expectedVarSet(varNames, varNames + 4),
				actualVarSet = pkb->getAllVarNames();
			Assert::IsTrue(expectedVarSet == actualVarSet);

			StmtNumber assigns[] = { 1, 2, 4, 5 };
			std::set<StmtNumber> expectedAssigns(assigns, assigns + 4),
				actualAssigns = pkb->getStmtsByType(ASSIGN);
			Assert::IsTrue(expectedAssigns == actualAssigns);

			StmtNumber usesZ[] = { 3, 4, 5 };
			std::set<StmtNumber> expectedUsesZ(usesZ, usesZ + 3),
				actualUsesZ = pkb->getStmtsByVar(USES, "z");
			Assert::IsTrue(expectedUsesZ == actualUsesZ);

			StmtNumber modifiesY[] = { 3, 5 };
			std::set<StmtNumber> expectedModifiesY(modifiesY, modifiesY + 2),
				actualModifiesY = pkb->getStmtsByVar(MODIFIES, "y");
			Assert::IsTrue(expectedModifiesY == actualModifiesY);

			StmtNumber stmt3[] = {2, 3};
			std::set<StmtNumber> expectedStmt3(stmt3, stmt3 + 2),
				actualStmt3 = pkb->getStmtsByStmt(FOLLOWS_STAR, 1);
			Assert::IsTrue(expectedStmt3 == actualStmt3);

			StmtNumber stmt4[] = { 3 };
			std::set<StmtNumber> expectedStmt4(stmt4, stmt4 + 1),
				actualStmt4 = pkb->getStmtsByStmt(5, PARENT_STAR);
			Assert::IsTrue(expectedStmt4 == actualStmt4);

			VarIndex usedBy3[] = { pkb->getVarIndex("z"), pkb->getVarIndex("x"), 
				pkb->getVarIndex("i") };
			std::set<VarIndex> expectedUsedBy3(usedBy3, usedBy3 + 3),
				actualUsedBy3 = pkb->getVarsByStmt(3, USES);
			Assert::IsTrue(expectedUsedBy3 == actualUsedBy3);
		}

		TEST_METHOD(TestPKB_LongProcedure) {
			PKB* pkb = PKB::getInstance();
			pkb->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("procedure_Panda.txt"));

			pkb = PKB::getInstance();

			StmtNumber parentStar10[] = { 4, 6, 8, 9 };
			std::set<StmtNumber> expectedParentStar10(parentStar10, parentStar10 + 4),
				actualParentStar10 = pkb->getStmtsByStmt(10, PARENT_STAR);
			Assert::IsTrue(expectedParentStar10 == actualParentStar10);

			StmtNumber usesX[] = { 2, 12, 14 };
			std::set<StmtNumber> expectedUsesX(usesX, usesX + 3),
				actualUsesX = pkb->getStmtsByVar(USES, "x");
			Assert::IsTrue(expectedUsesX == actualUsesX);

			VarIndex usedBy6[] = { pkb->getVarIndex("y"), pkb->getVarIndex("z"),
				pkb->getVarIndex("dumb"), pkb->getVarIndex("dumber") };

			std::set<VarIndex> expectedUsedBy6(usedBy6, usedBy6 + 4),
				actualUsedBy6 = pkb->getVarsByStmt(6, USES);
			Assert::IsTrue(expectedUsedBy6 == actualUsedBy6);
		}
	};
}