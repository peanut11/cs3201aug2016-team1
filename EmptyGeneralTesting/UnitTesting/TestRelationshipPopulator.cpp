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
		RelationshipPopulator *rp = RelationshipPopulator::getInstance();
		std::set<StmtNumber> actualSet = rp->getAndMemoiseNextStar(true, 1);
		
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
		actualSet = rp->getAndMemoiseNextStar(true, 8);

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
		actualSet = rp->getAndMemoiseNextStar(false, 8);

		actualSize = actualSet.size();
		expectedSize = 8;
		Assert::AreEqual(expectedSize, actualSize);

		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}
		pkb->clear();
		rp->clear();
	}

	TEST_METHOD(TestPopulator_Source) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("source.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();
		
		RelationshipPopulator *rp = RelationshipPopulator::getInstance();
		rp->clear();
		std::set<StmtNumber> actualSet = rp->getAndMemoiseNextStar(false, 1);
		actualSet = rp->getAndMemoiseNextStar(true, 2);
		Assert::IsTrue(rp->isNextStar(2, 5));
		Assert::IsTrue(true);
		pkb->clear();
		rp->clear();
	}

	TEST_METHOD(TestPopulator_GetAndMemoiseNextStar) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("program_RelationshipPop.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();

		// Proc One
		RelationshipPopulator *rp = RelationshipPopulator::getInstance();
		rp->clear();
		std::set<StmtNumber> actualSet = rp->getAndMemoiseNextStar(true, 1);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		StmtSetIterator actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 2);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}
		
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 3);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 4);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 5);
		Assert::AreEqual(StmtNumber(0), actualSet.size());

		// Proc Two
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 6);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 7);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 8);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Proc Three
		// Next* of if
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 9);
		Assert::AreEqual(StmtNumber(10), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 10; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in then list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 10);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 11; i <= 14; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 11);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 11; i <= 14; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 12);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 11; i <= 14; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in else list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 15);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 16; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 16);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 16; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 18);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 16; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Proc Four
		// Next* of if
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 20);
		Assert::AreEqual(StmtNumber(6), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in then list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 21);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 22);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 23);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in else list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 24);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 25);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(true, 26);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}
		pkb->clear();
		rp->clear();
	}

	TEST_METHOD(TestPopulator_GetAndMemoisePrevStar) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("program_RelationshipPop.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();

		// Proc One
		RelationshipPopulator *rp = RelationshipPopulator::getInstance();
		std::set<StmtNumber> actualSet = rp->getAndMemoiseNextStar(false, 5);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		StmtSetIterator actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 4);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 3);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 2);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 1);
		Assert::AreEqual(StmtNumber(0), actualSet.size());

		// Proc Two
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 6);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 7);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 8);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Proc Three
		// Prev* of if
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 9);
		Assert::AreEqual(StmtNumber(0), actualSet.size());

		// Prev* in then list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 14);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 13);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 12);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 11);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Prev* in else list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 19);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(9), *actualIt);
		actualIt++;
		for (unsigned int i = 15; i <= 18; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 18);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(9), *actualIt);
		actualIt++;
		for (unsigned int i = 15; i <= 18; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 17);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(9), *actualIt);
		actualIt++;
		for (unsigned int i = 15; i <= 18; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 16);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(9), *actualIt);
		actualIt++;
		for (unsigned int i = 15; i <= 18; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Proc Four
		// Prev* of then list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 23);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 20; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 22);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 20; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 21);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 20; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Prev* for else list
		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 26);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(20), *actualIt);
		actualIt++;
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 25);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(20), *actualIt);
		actualIt++;
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		rp->clear();
		rp = RelationshipPopulator::getInstance();
		actualSet = rp->getAndMemoiseNextStar(false, 24);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(20), *actualIt);
		actualIt++;
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		pkb->clear();
		rp->clear();
	}
	};
}
