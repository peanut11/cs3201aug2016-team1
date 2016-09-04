#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestProgramConverter) {
public:

	TEST_METHOD(TestConvert_ProcSecond) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		Assert::AreEqual(5, pc.convert(str));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_CheckRefTable) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		
		PKB* pkb = PKB::getInstance();
		std::vector<std::string> expected;
		expected.push_back("x");
		expected.push_back("i");
		expected.push_back("z");
		expected.push_back("y");
		std::vector<std::string> actual;
		actual = pkb->getAllVarNames();
		//Assert::AreEqual(expected.size(), actual.size());
		bool res = true;
		for (int i = 0; i < expected.size(); i++) {
			res = (expected[i] == actual[i]) && res;
		}
		Assert::IsTrue(res);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtModifies) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("x");
		Assert::IsTrue(pkb->is(1, MODIFIES, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtDoesNotModifies) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("i");
		Assert::IsFalse(pkb->is(1, MODIFIES, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtUses) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("i");
		Assert::IsTrue(pkb->is(4, USES, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtDoesNotUse) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("i");
		Assert::IsFalse(pkb->is(5, USES, x));
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtModifiesAndUses) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("z");
		bool modifies = pkb->is(4, MODIFIES, x);
		bool uses = pkb->is(4, USES, x);
		Assert::IsTrue(modifies && uses);
	}

	TEST_METHOD(TestUpdateAssignmentInTable_StmtWithConstants) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("0");
		Assert::IsFalse(pkb->is(1, USES, x));
	}
	};
}