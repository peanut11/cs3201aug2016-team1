#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestProgramValidator) {
public:

	TEST_METHOD(TestIsValidProgram_False_CallNotImplemented) {
		std::string str = "";
		str.append("procedure First {").append("\n");
		str.append("  x = 2;").append("\n");
		str.append("  z = 3;").append("\n");
		str.append("  call Second; }");

		ProgramValidator pv = ProgramValidator();
		Assert::IsFalse(pv.isValidProgram(str));
	}

	TEST_METHOD(TestIsValidProgram_True_While) {
		std::string str = "";
		str.append("procedure Second {").append("\n");
		str.append("  x = 0;").append("\n");
		str.append("  i = 5;").append("\n");
		str.append("  while i {").append("\n");
		str.append("    z = z + x + i;").append("\n");
		str.append("    y = z + 2; } }");

		ProgramValidator pv = ProgramValidator();
		Assert::IsTrue(pv.isValidProgram(str));
	}

	};
}