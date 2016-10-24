#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/RelationshipPopulator.h"
#include "../SPA/ProgramConverter.h"
#include "../SPA/DesignExtractor.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestRelationshipPopulator) {

public:
	TEST_METHOD(TestPopulator_NextStar) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("prototype_procedure_ConverterTest4.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();
		/*pkb->putStmtForStmt(StmtNumber(3), PARENT_STAR, StmtNumber(4));
		pkb->putStmtForStmt(StmtNumber(3), PARENT_STAR, StmtNumber(5));
		pkb->putStmtForStmt(StmtNumber(6), PARENT_STAR, StmtNumber(7));
		pkb->putStmtForStmt(StmtNumber(6), PARENT_STAR, StmtNumber(8));
		pkb->putStmtForStmt(StmtNumber(6), PARENT_STAR, StmtNumber(9));
		pkb->putStmtForStmt(StmtNumber(6), PARENT_STAR, StmtNumber(10));
		pkb->putStmtForStmt(StmtNumber(7), PARENT_STAR, StmtNumber(8));
*/
		RelationshipPopulator rp = RelationshipPopulator();
		std::set<StmtNumber> actualSet = rp.getAndMemoiseNextStar(true, 1);
		
		// Check Next*(1, 3) for no if/while
		//std::set<StmtNumber> actualSet = RelationshipPopulator::getNextStar(StmtNumber(1), StmtNumber(2));
		int actualSize = actualSet.size();
		int expectedSize = 10;
		//Assert::AreEqual(expectedSize, actualSize);

		std::set<StmtNumber>::const_iterator actualIt = actualSet.begin();
		for (unsigned int i = 2; i <=11; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Checks Next*(4, x) for if+nestedWhile
		actualSet = rp.getAndMemoiseNextStar(true, 8);

		actualSize = actualSet.size();
		expectedSize = 4;
		Assert::AreEqual(expectedSize, actualSize);

		actualIt = actualSet.begin();
		for (unsigned int i = 7; i <= 11; i++) {
			if (i != 10) {
				Assert::AreEqual(StmtNumber(i), *actualIt);
				actualIt++;
			}
		}

		// Checks Next*(x, 2) for no if/while
		actualSet = rp.getAndMemoiseNextStar(false, 8);

		actualSize = actualSet.size();
		expectedSize = 8;
		Assert::AreEqual(expectedSize, actualSize);

		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}
	}

	};
}
