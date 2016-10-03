#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestProgramValidator) {
public:

    TEST_METHOD(TestIsValidProgram_Stmt_Call) {
		try {
			std::string str = Tools::readFile("prototype_procedure_First.txt");

			ProgramValidator pv = ProgramValidator();
            Assert::IsTrue(pv.isValidSyntax(str));

		} catch (std::domain_error e) {
            Assert::AreEqual(Exception::INVALID_SIMPLE_SYNTAX.what(), e.what());
            Assert::Fail();
		}
	}

	TEST_METHOD(TestIsValidProgram_Stmt_While) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramValidator pv = ProgramValidator();
		Assert::IsTrue(pv.isValidSyntax(str));
	}

	};
}
