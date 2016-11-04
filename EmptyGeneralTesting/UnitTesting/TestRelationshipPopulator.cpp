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
		std::set<StmtNumber> actualSet = rp->getNextStar(1, 0);
		
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
		actualSet = rp->getNextStar(8, 0);

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
		actualSet = rp->getNextStar(0, 8);

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
		std::set<StmtNumber> actualSet = rp->getNextStar(1, 0);
		rp->getNextStar(1, 5);
		actualSet = rp->getNextStar(2, 0);
		Assert::IsTrue(true);
		pkb->clear();
		rp->clear();
	}

	TEST_METHOD(TestPopulator_GetNextStar) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("program_RelationshipPop.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();

		// Proc One
		RelationshipPopulator *rp = RelationshipPopulator::getInstance();
		//std::set<StmtNumber> actualSet = rp->getAndMemoiseNextStar(true, 1);
		std::set<StmtNumber> actualSet = rp->getNextStar(1, 0);
		//Assert::AreEqual(StmtNumber(4), actualSet.size());
		StmtSetIterator actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		//actualSet = rp->getAndMemoiseNextStar(true, 2);
		actualSet = rp->getNextStar(2, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}
		
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		//actualSet = rp->getAndMemoiseNextStar(true, 3);
		actualSet = rp->getNextStar(3, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		//actualSet = rp->getAndMemoiseNextStar(true, 4);
		actualSet = rp->getNextStar(4, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 2; i <= 5; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		//actualSet = rp->getAndMemoiseNextStar(true, 5);
		actualSet = rp->getNextStar(5, 0);
		Assert::AreEqual(StmtNumber(0), actualSet.size());

		// Proc Two
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(6, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(7, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(8, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Proc Three
		// Next* of if
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(9, 0);
		Assert::AreEqual(StmtNumber(10), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 10; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in then list
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(10, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 11; i <= 14; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(11, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 11; i <= 14; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(12, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 11; i <= 14; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in else list
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(15, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 16; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(16, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 16; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(18, 0);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 16; i <= 19; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Proc Four
		// Next* of if
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(20, 0);
		Assert::AreEqual(StmtNumber(6), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in then list
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(21, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(22, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(23, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 21; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Next* in else list
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(24, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(25, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(26, 0);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}
		pkb->clear();
		rp->clear();
	}

	TEST_METHOD(TestPopulator_GetPrevStar) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("program_RelationshipPop.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();

		// Proc One
		RelationshipPopulator *rp = RelationshipPopulator::getInstance();
		//std::set<StmtNumber> actualSet = rp->getAndMemoiseNextStar(false, 5);
		std::set<StmtNumber> actualSet = rp->getNextStar(0, 5);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		StmtSetIterator actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,4);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,3);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,2);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 1; i <= 4; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,1);
		Assert::AreEqual(StmtNumber(0), actualSet.size());

		// Proc Two
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,6);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,7);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,8);
		Assert::AreEqual(StmtNumber(3), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 6; i <= 8; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Proc Three
		// Prev* of if
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,9);
		Assert::AreEqual(StmtNumber(0), actualSet.size());

		// Prev* in then list
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,14);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,13);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,12);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,11);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 9; i <= 13; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Prev* in else list
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,19);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(9), *actualIt);
		actualIt++;
		for (unsigned int i = 15; i <= 18; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,18);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(9), *actualIt);
		actualIt++;
		for (unsigned int i = 15; i <= 18; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,17);
		Assert::AreEqual(StmtNumber(5), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(9), *actualIt);
		actualIt++;
		for (unsigned int i = 15; i <= 18; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,16);
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
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,23);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 20; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,22);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 20; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,21);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		for (unsigned int i = 20; i <= 23; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		// Prev* for else list
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,26);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(20), *actualIt);
		actualIt++;
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,25);
		Assert::AreEqual(StmtNumber(4), actualSet.size());
		actualIt = actualSet.begin();
		Assert::AreEqual(StmtNumber(20), *actualIt);
		actualIt++;
		for (unsigned int i = 24; i <= 26; i++) {
			Assert::AreEqual(StmtNumber(i), *actualIt);
			actualIt++;
		}

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(0,24);
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

	TEST_METHOD(TestPopulator_GetNextStarIs) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("program_RelationshipPop.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();
		RelationshipPopulator* rp = RelationshipPopulator::getInstance();

		// Proc One
		std::set<StmtNumber> actualSet = rp->getNextStar(1, 2);
		Assert::IsTrue(actualSet.find(2)!= actualSet.end());
		actualSet = rp->getNextStar(1, 3);
		Assert::IsTrue(actualSet.find(3) != actualSet.end());
		actualSet = rp->getNextStar(1, 4);
		Assert::IsTrue(actualSet.find(4) != actualSet.end());
		actualSet = rp->getNextStar(1, 5);
		Assert::IsTrue(actualSet.find(5) != actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(4, 5);
		Assert::IsTrue(actualSet.find(5) != actualSet.end());
		actualSet = rp->getNextStar(3, 5);
		Assert::IsTrue(actualSet.find(5) != actualSet.end());
		actualSet = rp->getNextStar(2, 5);
		Assert::IsTrue(actualSet.find(5) != actualSet.end());
		actualSet = rp->getNextStar(1, 5);
		Assert::IsTrue(actualSet.find(5) != actualSet.end());

		// Proc Two
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(6, 7);
		Assert::IsTrue(actualSet.find(7) != actualSet.end());
		actualSet = rp->getNextStar(6, 8);
		Assert::IsTrue(actualSet.find(8) != actualSet.end());
		actualSet = rp->getNextStar(6, 6);
		Assert::IsTrue(actualSet.find(6) != actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(7, 6);
		Assert::IsTrue(actualSet.find(6) != actualSet.end());
		actualSet = rp->getNextStar(7, 8);
		Assert::IsTrue(actualSet.find(8) != actualSet.end());
		actualSet = rp->getNextStar(7, 7);
		Assert::IsTrue(actualSet.find(7) != actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(8, 6);
		Assert::IsTrue(actualSet.find(6) != actualSet.end());
		actualSet = rp->getNextStar(8, 8);
		Assert::IsTrue(actualSet.find(8) != actualSet.end());
		actualSet = rp->getNextStar(8, 7);
		Assert::IsTrue(actualSet.find(7) != actualSet.end());

		// Proc Three
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(9, 10);
		Assert::IsTrue(actualSet.find(10) != actualSet.end());
		actualSet = rp->getNextStar(9, 11);
		Assert::IsTrue(actualSet.find(11) != actualSet.end());
		actualSet = rp->getNextStar(9, 12);
		Assert::IsTrue(actualSet.find(12) != actualSet.end());
		actualSet = rp->getNextStar(9, 13);
		Assert::IsTrue(actualSet.find(13) != actualSet.end());
		actualSet = rp->getNextStar(9, 14);
		Assert::IsTrue(actualSet.find(14) != actualSet.end());
		actualSet = rp->getNextStar(9, 15);
		Assert::IsTrue(actualSet.find(15) != actualSet.end());
		actualSet = rp->getNextStar(9, 16);
		Assert::IsTrue(actualSet.find(16) != actualSet.end());
		actualSet = rp->getNextStar(9, 17);
		Assert::IsTrue(actualSet.find(17) != actualSet.end());
		actualSet = rp->getNextStar(9, 18);
		Assert::IsTrue(actualSet.find(18) != actualSet.end());
		actualSet = rp->getNextStar(9, 19);
		Assert::IsTrue(actualSet.find(19) != actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(10, 11);
		Assert::IsTrue(actualSet.find(11) != actualSet.end());
		actualSet = rp->getNextStar(10, 12);
		Assert::IsTrue(actualSet.find(12) != actualSet.end());
		actualSet = rp->getNextStar(10, 13);
		Assert::IsTrue(actualSet.find(13) != actualSet.end());
		actualSet = rp->getNextStar(10, 14);
		Assert::IsTrue(actualSet.find(14) != actualSet.end());
		actualSet = rp->getNextStar(10, 15);
		Assert::IsTrue(actualSet.find(15) == actualSet.end());
		actualSet = rp->getNextStar(10, 16);
		Assert::IsTrue(actualSet.find(16) == actualSet.end());
		actualSet = rp->getNextStar(10, 17);
		Assert::IsTrue(actualSet.find(17) == actualSet.end());
		actualSet = rp->getNextStar(10, 18);
		Assert::IsTrue(actualSet.find(18) == actualSet.end());
		actualSet = rp->getNextStar(10, 19);
		Assert::IsTrue(actualSet.find(19) == actualSet.end());

		// Proc Four
		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(20, 21);
		Assert::IsTrue(actualSet.find(21) != actualSet.end());
		actualSet = rp->getNextStar(20, 22);
		Assert::IsTrue(actualSet.find(22) != actualSet.end());
		actualSet = rp->getNextStar(20, 23);
		Assert::IsTrue(actualSet.find(23) != actualSet.end());
		actualSet = rp->getNextStar(20, 24);
		Assert::IsTrue(actualSet.find(24) != actualSet.end());
		actualSet = rp->getNextStar(20, 25);
		Assert::IsTrue(actualSet.find(25) != actualSet.end());
		actualSet = rp->getNextStar(20, 26);
		Assert::IsTrue(actualSet.find(26) != actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(21, 21);
		Assert::IsTrue(actualSet.find(21) != actualSet.end());
		actualSet = rp->getNextStar(21, 22);
		Assert::IsTrue(actualSet.find(22) != actualSet.end());
		actualSet = rp->getNextStar(21, 23);
		Assert::IsTrue(actualSet.find(23) != actualSet.end());
		actualSet = rp->getNextStar(21, 24);
		Assert::IsTrue(actualSet.find(24) == actualSet.end());
		actualSet = rp->getNextStar(21, 25);
		Assert::IsTrue(actualSet.find(25) == actualSet.end());
		actualSet = rp->getNextStar(21, 26);
		Assert::IsTrue(actualSet.find(26) == actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(22, 21);
		Assert::IsTrue(actualSet.find(21) != actualSet.end());
		actualSet = rp->getNextStar(22, 22);
		Assert::IsTrue(actualSet.find(22) != actualSet.end());
		actualSet = rp->getNextStar(22, 23);
		Assert::IsTrue(actualSet.find(23) != actualSet.end());
		actualSet = rp->getNextStar(22, 24);
		Assert::IsTrue(actualSet.find(24) == actualSet.end());
		actualSet = rp->getNextStar(22, 25);
		Assert::IsTrue(actualSet.find(25) == actualSet.end());
		actualSet = rp->getNextStar(22, 26);
		Assert::IsTrue(actualSet.find(26) == actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(24, 21);
		Assert::IsTrue(actualSet.find(21) == actualSet.end());
		actualSet = rp->getNextStar(24, 22);
		Assert::IsTrue(actualSet.find(22) == actualSet.end());
		actualSet = rp->getNextStar(24, 23);
		Assert::IsTrue(actualSet.find(23) == actualSet.end());
		actualSet = rp->getNextStar(24, 24);
		Assert::IsTrue(actualSet.find(24) != actualSet.end());
		actualSet = rp->getNextStar(24, 25);
		Assert::IsTrue(actualSet.find(25) != actualSet.end());
		actualSet = rp->getNextStar(24, 26);
		Assert::IsTrue(actualSet.find(26) != actualSet.end());

		//rp->clear();
		//rp = RelationshipPopulator::getInstance();
		actualSet = rp->getNextStar(25, 21);
		Assert::IsTrue(actualSet.find(21) == actualSet.end());
		actualSet = rp->getNextStar(25, 22);
		Assert::IsTrue(actualSet.find(22) == actualSet.end());
		actualSet = rp->getNextStar(25, 23);
		Assert::IsTrue(actualSet.find(23) == actualSet.end());
		actualSet = rp->getNextStar(25, 24);
		Assert::IsTrue(actualSet.find(24) != actualSet.end());
		actualSet = rp->getNextStar(25, 25);
		Assert::IsTrue(actualSet.find(25) != actualSet.end());
		actualSet = rp->getNextStar(25, 26);
		Assert::IsTrue(actualSet.find(26) != actualSet.end());

		// Diff Procs
		actualSet = rp->getNextStar(1, 26);
		Assert::IsTrue(actualSet.find(26) == actualSet.end());

		rp->clear();
		pkb->clear();
	}
	};
}

