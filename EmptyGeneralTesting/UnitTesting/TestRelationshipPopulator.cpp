#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/RelationshipPopulator.h"
#include "../SPA/ProgramConverter.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestRelationshipPopulator) {

public:
	TEST_METHOD(TestPopulator_NextStar) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("prototype_procedure_First.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		// Check Next*(1, 3) for no if/while
		std::set<StmtNumber> actualSet = RelationshipPopulator::getNextStar(StmtNumber(1), StmtNumber(2));
		int actualSize = actualSet.size();
		int expectedSize = 1;
		Assert::AreEqual(expectedSize, actualSize);

		std::set<StmtNumber>::const_iterator actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 2; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Checks Next*(1, x) for no if/while
		actualSet = RelationshipPopulator::getNextStar(StmtNumber(1), StmtNumber(0));

		actualSize = actualSet.size();
		expectedSize = 2;
		Assert::AreEqual(expectedSize, actualSize);

		actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 3; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Checks Next*(x, 2) for no if/while
		/*actualSet = RelationshipPopulator::getNextStar(StmtNumber(0), StmtNumber(3));

		actualSize = actualSet.size();
		expectedSize = 2;
		Assert::AreEqual(expectedSize, actualSize);

		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 2; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}*/
	}

	};
}
