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

	};
}