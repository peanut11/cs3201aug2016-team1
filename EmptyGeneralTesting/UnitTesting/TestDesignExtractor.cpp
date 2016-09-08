#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/ProgramConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestDesignExtractor) {
		TEST_CLASS_INITIALIZE(initialize) {
			std::string str = Tools::readFile("prototype_procedure_Third.txt");

			ProgramConverter pc = ProgramConverter();
			pc.convert(str);
			PKB* pkb = PKB::getInstance();
			
			DesignExtractor test = DesignExtractor(); 
			test.process();
			std::set<StmtNumber> i = pkb->getStmtsByStmt(0, FOLLOWSSTAR);
			int a = i.size();
			int expected = 2;
			Assert::AreEqual(a, 2);
		}
	};

}