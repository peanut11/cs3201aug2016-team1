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

	TEST_METHOD(TestUpdateAssignmentInTable_ByVarNames) {
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
		Assert::AreEqual(expected.size(), actual.size());

		for (int i = 0; i < expected.size(); i++) {
			Assert::AreEqual(expected[i], actual[i]);
		}
	}

	TEST_METHOD(TestUpdateAssignmentInTable_ByIsRelations) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		PKB* pkb = PKB::getInstance();
		VarIndex x = pkb->getVarIndex("x");
		Assert::IsTrue(pkb->is(1, MODIFIES, x));
		// TODO: Incomplete
	}

	};
}