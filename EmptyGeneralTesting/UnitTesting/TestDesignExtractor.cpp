#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/ProgramConverter.h"
#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
namespace UnitTesting {
	TEST_CLASS(TestDesignExtractor) {
		TEST_CLASS_INITIALIZE(initialize) {
			std::string str = Tools::readFile("prototype_procedure_Third.txt");

			ProgramConverter pc = ProgramConverter();
			pc.convert(str);

			DesignExtractor de = DesignExtractor();
			de.process();
		};

public:
	TEST_METHOD(TestDesign_FollowedByStar) {
		

		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> followedByStarList = pkb->getStmtsByStmt(1, FOLLOWED_BY_STAR);
		
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
		followedByStarList = pkb->getStmtsByStmt(4, FOLLOWED_BY_STAR);
		followedByStarIt = followedByStarList.begin();
		followedByStarSize = followedByStarList.size();
		Assert::AreEqual(followedByStarSize, 2);
		test = *followedByStarIt;
		Assert::AreEqual(test, 5);
		followedByStarIt++;
		test = *followedByStarIt;
		Assert::AreEqual(test, 6);
		//std::set<StmtNumber> k= pkb->getStmtsByStmt(5, PARENT);
		//StmtSetIterator it = k.begin();
		//it++;
		//int a = *it;
		//Assert::AreEqual(0, a);
		//int a = k.begin();
		//int expected = 2;
		//int b = 5;
		//if (a == expected) {
			//for (int j = 0; j < a; j++) {

			//}
		//}
		
		//std::set<VarName> actual;
		//actual = pkb->getAllVarNames();
		//int f = k.size();
		//Assert::AreEqual(l,2);
		//l++;
		//Assert::AreEqual(f, 5);

		//Assert::AreEqual(f, 0);
	}/*
	TEST_METHOD(TestDesign_FollowsStar) {


		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> followsStarList = pkb->getStmtsByStmt(12,FOLLOWS_STAR);

		StmtSetIterator followsStarIt = followsStarList.begin();

		int followsStarSize = followsStarList.size();
		Assert::AreEqual(followsStarSize, 6);
		int test = *followsStarIt;
		Assert::AreEqual(test, 1);
		followsStarIt++;
		test = *followsStarIt;
		Assert::AreEqual(test, 2);
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
		Assert::AreEqual(test, 11);
		
		//std::set<StmtNumber> k= pkb->getStmtsByStmt(5, PARENT);
		//StmtSetIterator it = k.begin();
		//it++;
		//int a = *it;
		//Assert::AreEqual(0, a);
		//int a = k.begin();
		//int expected = 2;
		//int b = 5;
		//if (a == expected) {
		//for (int j = 0; j < a; j++) {

		//}
		//}

		//std::set<VarName> actual;
		//actual = pkb->getAllVarNames();
		//int f = k.size();
		//Assert::AreEqual(l,2);
		//l++;
		//Assert::AreEqual(f, 5);

		//Assert::AreEqual(f, 0);
	}*/
	TEST_METHOD(TestDesign_PARENT_STAR) {
		

		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> PARENT_STARList = pkb->getStmtsByStmt(1, PARENT_STAR);
		StmtSetIterator PARENT_STARIt = PARENT_STARList.begin();

		int PARENT_STARSize = PARENT_STARList.size();
		Assert::AreEqual(PARENT_STARSize, 0);
		PARENT_STARList = pkb->getStmtsByStmt(7, PARENT_STAR);
		PARENT_STARIt = PARENT_STARList.begin();

		PARENT_STARSize = PARENT_STARList.size();
		Assert::AreEqual(PARENT_STARSize, 2);
		int test = *PARENT_STARIt;
		Assert::AreEqual(test, 3);
		PARENT_STARIt++;
		test = *PARENT_STARIt;
		Assert::AreEqual(test, 6);
	}
	TEST_METHOD(TestDesign_ParentOFStar) {
		
		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> parentOfStarList = pkb->getStmtsByStmt(1, PARENT_OF_STAR);
		StmtSetIterator PARENT_STARIt = parentOfStarList.begin();
		int PARENT_STARSize = parentOfStarList.size();
		Assert::AreEqual(PARENT_STARSize, 0);
		parentOfStarList = pkb->getStmtsByStmt(3, PARENT_OF_STAR);
		PARENT_STARIt = parentOfStarList.begin();
		PARENT_STARSize = parentOfStarList.size();
		Assert::AreEqual(PARENT_STARSize, 5);
	}
	TEST_METHOD(TestDesign_UpdateStmtTable) {
		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> useList = pkb->getVarsByStmt(6, MODIFIES);
		StmtSetIterator useIt = useList.begin();
		DesignExtractor de = DesignExtractor();
		int useSize = de.getwhileList();
		Assert::AreEqual(useSize, 2);
		int sz = useList.size();
		Assert::AreEqual(sz, 2);
		 useList = pkb->getVarsByStmt(3, USES);
		 useIt = useList.begin();
		 sz = useList.size();
		Assert::AreEqual(sz, 4);
	}
	
	};
}