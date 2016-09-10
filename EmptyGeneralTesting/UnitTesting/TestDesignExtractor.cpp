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
	}
	TEST_METHOD(TestDesign_ParentStar) {
		

		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> parentStarList = pkb->getStmtsByStmt(1, PARENTSTAR);
		StmtSetIterator parentStarIt = parentStarList.begin();

		int parentStarSize = parentStarList.size();
		Assert::AreEqual(parentStarSize, 0);
		parentStarList = pkb->getStmtsByStmt(7, PARENTSTAR);
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
		std::set<StmtNumber> parentOfStarList = pkb->getStmtsByStmt(1, PARENT_OF_STAR);
		StmtSetIterator parentStarIt = parentOfStarList.begin();
		int parentStarSize = parentOfStarList.size();
		Assert::AreEqual(parentStarSize, 0);
		parentOfStarList = pkb->getStmtsByStmt(3, PARENT_OF_STAR);
		parentStarIt = parentOfStarList.begin();
		parentStarSize = parentOfStarList.size();
		Assert::AreEqual(parentStarSize, 5);
	}
	};
}