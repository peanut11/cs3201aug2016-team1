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
		

		DesignExtractor test = DesignExtractor();
		test.process();
		PKB* pkb = PKB::getInstance();
		std::set<StmtNumber> i = pkb->getStmtsByStmt(1, FOLLOWSSTAR);
		std::set<StmtNumber> k= pkb->getStmtsByStmt(1, FOLLOWS);
		int a = k.size();
		int expected = 2;
		int b = 5;
		if (a == expected) {
			for (int j = 0; j < a; j++) {

			}
		}
		
		std::set<VarName> actual;
		actual = pkb->getAllVarNames();
		int f = actual.size();
		Assert::AreEqual(a, 0);
		Assert::AreEqual(f, 0);
	}
	};
}