#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/ProgramConverter.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestDesignExtractor) {

		TEST_CLASS_INITIALIZE(Initialize) {
			std::string str = Tools::readFile("prototype_procedure_Third.txt");

			ProgramConverter pc = ProgramConverter();
			pc.convert(str);

			DesignExtractor de = DesignExtractor();
			de.process();
		};

public:
	TEST_METHOD(TestDesign_FollowedByStar) {
		PKB* pkb = PKB::getInstance();

		std::set<StmtNumber> followedByStarList = pkb->getStmtsByStmt(FOLLOWS_STAR,1);
		StmtSetIterator followedByStarIt = followedByStarList.begin();

		int followedByStarSize = followedByStarList.size();
		Assert::AreEqual(followedByStarSize, 3);

		int test = *followedByStarIt;
		Assert::AreEqual(test, 2);

		followedByStarIt++;
		test = *followedByStarIt;
		Assert::AreEqual(test, 3);

		followedByStarIt++;
		test = *followedByStarIt;
		Assert::AreEqual(test, 9);

		followedByStarList = pkb->getStmtsByStmt(FOLLOWS_STAR,4);
		followedByStarIt = followedByStarList.begin();

		followedByStarSize = followedByStarList.size();
		Assert::AreEqual(followedByStarSize, 2);

		test = *followedByStarIt;
		Assert::AreEqual(test, 5);

		followedByStarIt++;
		test = *followedByStarIt;
		Assert::AreEqual(test, 6);
	}

	TEST_METHOD(TestDesign_FollowsStar) {
		PKB* pkb = PKB::getInstance();

		std::set<StmtNumber> followsStarList = pkb->getStmtsByStmt(FOLLOWS_STAR, 2); // ERROR: No stmt 12
		StmtSetIterator followsStarIt = followsStarList.begin();

		int followsStarSize = followsStarList.size();
		Assert::AreEqual(2, followsStarSize);
		
		int test = *followsStarIt;
		Assert::AreEqual(test, 3);
		
		followsStarIt++;
		test = *followsStarIt;
		Assert::AreEqual(test, 9);
		/*
		followsStarIt++;
		test = *followsStarIt;
		Assert::AreEqual(test, 3);

		followsStarIt++;
		test = *followsStarIt;
		Assert::AreEqual(test, 4);

		followsStarIt++;
		test = *followsStarIt;
		Assert::AreEqual(test, 5);

		followsStarIt++;
		test = *followsStarIt;
		Assert::AreEqual(test, 11);*/
	}

	TEST_METHOD(TestDesign_ParentStar) {
		PKB* pkb = PKB::getInstance();

		std::set<StmtNumber> parentStarList = pkb->getStmtsByStmt(1, PARENT_STAR);
		StmtSetIterator parentStarIt = parentStarList.begin();

		int parentStarSize = parentStarList.size();
		Assert::AreEqual(parentStarSize, 0);

		parentStarList = pkb->getStmtsByStmt(7, PARENT_STAR);
		parentStarIt = parentStarList.begin();

		parentStarSize = parentStarList.size();
		Assert::AreEqual(parentStarSize, 2);

		int test = *parentStarIt;
		Assert::AreEqual(test, 3);

		parentStarIt++;
		test = *parentStarIt;
		Assert::AreEqual(test, 6);
	}

	TEST_METHOD(TestDesign_ParentOFStar) {
		PKB* pkb = PKB::getInstance();

		std::set<StmtNumber> parentOfStarList = pkb->getStmtsByStmt(PARENT_STAR,1);
		StmtSetIterator parentStarIt = parentOfStarList.begin();

		int parentStarSize = parentOfStarList.size();
		Assert::AreEqual(parentStarSize, 0);

		parentOfStarList = pkb->getStmtsByStmt(PARENT_STAR,3);
		parentStarIt = parentOfStarList.begin();
		parentStarSize = parentOfStarList.size();
		Assert::AreEqual(parentStarSize, 5);
		int test = *parentStarIt;
		Assert::AreEqual(test, 4);
	}

	TEST_METHOD(TestDesign_UpdateStmtTable) {
		PKB* pkb = PKB::getInstance();
        DesignExtractor de = DesignExtractor();

		int whileSize = de.getWhileListSize();
		Assert::AreEqual(2, whileSize);

		int modifiesSize = pkb->getVarsByStmt(6, MODIFIES).size();
        Assert::AreEqual(size_t(2), pkb->getStmtsByStmt(PARENT, 6).size());
		Assert::AreEqual(2, modifiesSize);

        int usesSize = pkb->getVarsByStmt(3, USES).size();
		Assert::AreEqual(4, usesSize);
	}
	
	};
}
