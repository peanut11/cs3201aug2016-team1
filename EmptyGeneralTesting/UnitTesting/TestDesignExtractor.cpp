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

		};

public:
	TEST_METHOD(TestDesign) {
		std::string str = Tools::readFile("prototype_procedure_Third.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		

		DesignExtractor de = DesignExtractor();
		de.process();
		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> i = pkb->getStmtsByStmt(1, FOLLOWED_BY_STAR);
		std::set<StmtNumber> k= pkb->getStmtsByStmt(5, PARENT);
		StmtSetIterator it = k.begin();
		//it++;
		StmtSetIterator follo = i.begin();

		int l = *follo;
		int a = *it;
		Assert::AreEqual(0, a);
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
	};
}