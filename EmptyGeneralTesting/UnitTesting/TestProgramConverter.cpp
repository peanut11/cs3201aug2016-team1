#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	
	TEST_CLASS(TestProgramConverter) {
		
	TEST_METHOD_INITIALIZE(initialize) {
		std::string str = Tools::readFile("prototype_procedure_ConverterTest.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
	};

	TEST_METHOD_CLEANUP(destruct) {
		PKB* pkb = PKB::getInstance();
		pkb->clear();
	};

public:
	TEST_METHOD(TestConvert_ProcSecond) {
		PKB* pkb = PKB::getInstance();
		pkb->clear();
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		int progLines = pc.convert(str);
		Assert::AreEqual(5, progLines);
	}
	
	TEST_METHOD(TestUpdateStmtInStmtTable_CheckFirstLineFollows) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 1;
		
		std::set<StmtNumber> expected; //should be empty
		std::set<StmtNumber> actual; 
		
		actual = pkb->getStmtsByStmt(stmt1, FOLLOWS);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckRegularFollows) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 1;
		StmtNumber stmt2 = 2;

		std::set<StmtNumber> expected; // {stmt1}
		std::set<StmtNumber> actual;

		expected.insert(stmt1);

		actual = pkb->getStmtsByStmt(stmt2, FOLLOWS);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckFirstLineFollowedBy) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 1;
		StmtNumber stmt2 = 2;

		std::set<StmtNumber> expected; // {stmt1}
		std::set<StmtNumber> actual;

		expected.insert(stmt2);

		actual = pkb->getStmtsByStmt(FOLLOWS, stmt1);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckFirstLineWhileFollows) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 4;

		std::set<StmtNumber> expected; // {}
		std::set<StmtNumber> actual;

		actual = pkb->getStmtsByStmt(stmt1, FOLLOWS);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckAfterWhileFollows) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 6;
		StmtNumber stmt2 = 3;

		std::set<StmtNumber> expected; // {stmt2}
		std::set<StmtNumber> actual;
		expected.insert(stmt2);
		actual = pkb->getStmtsByStmt(stmt1, FOLLOWS);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckWhileFollowBy) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 3;
		StmtNumber stmt2 = 6;

		std::set<StmtNumber> expected; // {stmt2}
		std::set<StmtNumber> actual;
		expected.insert(stmt2);
		actual = pkb->getStmtsByStmt(FOLLOWS, stmt1);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckFirstLineParent) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 1;

		std::set<StmtNumber> expected; // {}
		std::set<StmtNumber> actual;
		
		actual = pkb->getStmtsByStmt(stmt1, PARENT);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckInsideWhileParent) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt1 = 3;
		StmtNumber stmt2 = 4;
		std::set<StmtNumber> expected; // {stmt1}
		std::set<StmtNumber> actual;
		expected.insert(stmt1);
		actual = pkb->getStmtsByStmt(stmt2, PARENT);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckWhileIsParentOf) {

		PKB* pkb = PKB::getInstance();
		StmtNumber stmt3 = 3;
		StmtNumber stmt4 = 4;
		StmtNumber stmt5 = 5;
		std::set<StmtNumber> expected; // {4, 5}
		std::set<StmtNumber> actual;
		expected.insert(stmt4);
		expected.insert(stmt5);
		actual = pkb->getStmtsByStmt(PARENT, stmt3);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckStmtType) {

		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> expected; // {a, a, w, a, a, a}
		std::set<StmtNumber> actual;
		expected.insert(ASSIGN);
		expected.insert(ASSIGN);
		expected.insert(WHILE);
		expected.insert(ASSIGN);
		expected.insert(ASSIGN);
		expected.insert(ASSIGN);

		for (unsigned int stmt = 1; stmt < pkb->getStmtTableSize(); stmt++) {
			actual.insert(pkb->getStmtTypeForStmt(stmt));
		}
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_CheckRefTable) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/
		
		PKB* pkb = PKB::getInstance();
		std::set<VarName> expected;
		expected.insert("x");
		expected.insert("i");
		expected.insert("z");
		expected.insert("y");
		std::set<VarName> actual;
		actual = pkb->getAllVarNames();
		
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_WhileUses) {
		PKB* pkb = PKB::getInstance();
		StmtNumber whi = 3;
		VarIndex i = pkb->getVarIndex("i");
		Assert::IsTrue(pkb->is(USES, whi, i));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_VarUsedByWhile) {
		PKB* pkb = PKB::getInstance();
		StmtNumber whi = 3;
		StmtNumber stmt = 4;
		std::set<StmtNumber> actual;
		std::set<StmtNumber> expected;
		expected.insert(whi);
		expected.insert(stmt);
		actual = pkb->getStmtsByVar(USES, "i");
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtModifies) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("x");
		Assert::IsTrue(pkb->is(MODIFIES, 1, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtDoesNotModifies) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("i");
		Assert::IsFalse(pkb->is(MODIFIES, 1, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtUses) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("i");
		Assert::IsTrue(pkb->is(USES, 4, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtDoesNotUse) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("i");
		Assert::IsFalse(pkb->is(USES, 5, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtModifiesAndUses) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("z");
		bool modifies = pkb->is(MODIFIES, 4, x);
		bool uses = pkb->is(USES, 4, x);
		Assert::IsTrue(modifies && uses);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtWithConstants) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("0");
		Assert::IsFalse(pkb->is(USES, 1, x));
	}
	
	TEST_METHOD(TestUpdateAssignmentInTable_VarModifiedBy) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();
		
		std::set<StmtNumber> output = pkb->getStmtsByVar(MODIFIES, "x");
		std::set<StmtNumber> expectedOutput;
		StmtNumber stmtNo = 1;
		StmtNumber stmtNo2 = 6;
		expectedOutput.insert(stmtNo);
		expectedOutput.insert(stmtNo2);

		Assert::IsTrue(expectedOutput == output);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_VarUsedBy) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();

		std::set<StmtNumber> output = pkb->getStmtsByVar(USES, "x");
		std::set<StmtNumber> expectedOutput;
		StmtNumber stmtNo = 4;
		expectedOutput.insert(stmtNo);

		Assert::IsTrue(output == expectedOutput);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_Constants) {
		Constant const1 = 0;
		Constant const2 = 5;
		Constant const3 = 2;
		Constant const4 = 7;

        std::set<Constant> expected({ const1, const2, const3, const4 });
        std::set<Constant> actual = PKB::getInstance()->getAllConstantValues();

        Assert::AreEqual(expected.size(), actual.size());

        std::set<Constant>::const_iterator expectedIt = expected.begin();
        std::set<Constant>::const_iterator actualIt = actual.begin();

        while (expectedIt != expected.end()) {
            Assert::AreEqual(*expectedIt, *actualIt);

            expectedIt++;
            actualIt++;
        }
	}
	};
}
