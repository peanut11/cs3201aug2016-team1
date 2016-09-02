#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestProgramValidator) {
public:

	TEST_METHOD(TestIsValidProgram_False_CallNotImplemented) {
		std::string str = Tools::readFile("prototype_procedure_First.txt");

		ProgramValidator pv = ProgramValidator();
		Assert::IsFalse(pv.isValidProgram(str));
	}

	TEST_METHOD(TestIsValidProgram_True_While) {
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramValidator pv = ProgramValidator();
		Assert::IsTrue(pv.isValidProgram(str));
	}

	};
}
