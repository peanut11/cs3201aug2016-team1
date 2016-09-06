#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	
	TEST_CLASS(TestProgramConverter) {
		
	TEST_CLASS_INITIALIZE(initialize) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
	};

	/*TEST_METHOD_CLEANUP(destruct) {
		PKB* pkb = PKB::getInstance();
		delete pkb;
	};*/

public:
	TEST_METHOD(TestConvert_ProcSecond) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		Assert::AreEqual(5, pc.convert(str));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_CheckRefTable) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/
		
		PKB* pkb = PKB::getInstance();
		std::vector<std::string> expected;
		expected.push_back("x");
		expected.push_back("i");
		expected.push_back("z");
		expected.push_back("y");
		std::vector<std::string> actual;
		actual = pkb->getAllVarNames();
		
		bool res = expected.size() == actual.size();
		for (unsigned int i = 0; i < expected.size(); i++) {
			res = (expected[i] == actual[i]) && res;
		}
		Assert::IsTrue(res);
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
		
		std::vector<StmtNumber> output = pkb->getStmtsByVar(MODIFIES, "x");
		std::vector<StmtNumber> expectedOutput;
		StmtNumber stmtNo = 1;
		expectedOutput.push_back(stmtNo);

		bool res = expectedOutput.size() == output.size();
		for (unsigned int i = 0; i < expectedOutput.size(); i++) {
			res = (expectedOutput[i] == output[i]) && res;
		}
		Assert::IsTrue(res);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_VarUsedBy) {
		/*std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);*/

		PKB* pkb = PKB::getInstance();

		std::vector<StmtNumber> output = pkb->getStmtsByVar(USES, "x");
		std::vector<StmtNumber> expectedOutput;
		StmtNumber stmtNo = 4;
		expectedOutput.push_back(stmtNo);

		bool res = expectedOutput.size() == output.size();
		for (unsigned int i = 0; i < expectedOutput.size(); i++) {
			res = (expectedOutput[i] == output[i]) && res;
		}
		Assert::IsTrue(res);
	}
	};
}