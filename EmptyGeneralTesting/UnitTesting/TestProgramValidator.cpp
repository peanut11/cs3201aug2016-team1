#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestProgramValidator) {
public:

    TEST_METHOD(TestIsValidProgram_Proc_MultipleProc) {
        try {
            std::string str = Tools::readFile("program_MultipleProcedure.txt");

            ProgramValidator pv = ProgramValidator();
            Assert::IsTrue(pv.isValidSyntax(str));

        } catch (std::domain_error e) {
            Assert::AreEqual(Exception::INVALID_SIMPLE_SYNTAX.what(), e.what());
            Assert::Fail();
        }
    }
    
    TEST_METHOD(TestIsValidProgram_Stmt_Call_Nonexisting) {
		try {
			std::string str = Tools::readFile("prototype_procedure_First.txt");

			ProgramValidator pv = ProgramValidator();
            Assert::IsTrue(pv.isValidSyntax(str));
            Assert::Fail();

		} catch (std::domain_error e) {
            Assert::AreEqual(Exception::INVALID_CALL_MADE.what(), e.what());
		}
	}

    TEST_METHOD(TestIsValidProgram_Stmt_Call_Recursive) {
        try {
            std::string str = Tools::readFile("invalid_program_RecursiveCall_ProgramValidator.txt");

            ProgramValidator pv = ProgramValidator();
            Assert::IsFalse(pv.isValidSyntax(str));
            Assert::Fail();

        } catch (std::invalid_argument e) {
            Assert::AreEqual(Exception::RECURSIVE_CALL_ERROR.what(), e.what());
        }
    }

	TEST_METHOD(TestIsValidProgram_Stmt_While) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramValidator pv = ProgramValidator();
		Assert::IsTrue(pv.isValidSyntax(str));
	}

	};
}
