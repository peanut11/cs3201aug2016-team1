#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/AffectsCalculator.h"
#include "../SPA/ProgramConverter.h"
#include "../SPA/DesignExtractor.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestAffectsCalculator) {

public:
	TEST_METHOD(TestAffects) {
		PKB::getInstance()->clear();
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("source.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();

		AffectsCalculator::getInstance()->clear();
		AffectsCalculator * ac = AffectsCalculator::getInstance();

		Assert::IsTrue(pkb->is(AFFECTS, 21, 38));
		Assert::IsTrue(pkb->is(AFFECTS_STAR, 20, 49));
		Assert::IsFalse(pkb->is(AFFECTS, 44, 47));
		Assert::IsTrue(pkb->is(AFFECTS_STAR, 44, 47));

		StmtSet actualSet = pkb->getStmtsByStmt(AFFECTS, 3);
		StmtSet expectedSet;
		expectedSet.insert(6);

		Assert::IsTrue(actualSet == expectedSet);

		actualSet = pkb->getStmtsByStmt(AFFECTS, 1);
		expectedSet.clear();
		Assert::IsTrue(actualSet == expectedSet);

		actualSet = pkb->getStmtsByStmt(AFFECTS_STAR, 192);
		expectedSet.clear();
		expectedSet.insert(193);

		Assert::IsTrue(actualSet == expectedSet);

		actualSet = pkb->getStmtsByStmt(140, AFFECTS_STAR);
		expectedSet.clear();
		expectedSet.insert(129);
		expectedSet.insert(132);
		expectedSet.insert(133);
		for (int i = 137; i <= 139; i++) expectedSet.insert(i);

		/*		Debug template
		char c[10];
		Logger::WriteMessage("=======");
		for (StmtNumber s : actualSet) Logger::WriteMessage(itoa(s, c, 10));
		Logger::WriteMessage("======="); */

		Assert::IsTrue(pkb->is(MODIFIES, 128, pkb->getVarIndex("x")));
		Assert::IsTrue(actualSet == expectedSet);

		actualSet = pkb->getStmtsByStmt(139, AFFECTS);
		expectedSet.clear();
		expectedSet.insert(129);
		expectedSet.insert(132);
		expectedSet.insert(133);
		expectedSet.insert(137);
		expectedSet.insert(138);

		char c[10];
		Logger::WriteMessage("=======");
		for (StmtNumber s : actualSet) Logger::WriteMessage(itoa(s, c, 10));
		Logger::WriteMessage("=======");

		Assert::IsFalse(pkb->is(MODIFIES, 130, pkb->getVarIndex("i")));
		Assert::IsTrue(actualSet == expectedSet);

		pkb->clear();
		ac->clear();
	}

	TEST_METHOD(TestAffects_SourceB) {
		PKB::getInstance()->clear();
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("sourceB.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		DesignExtractor de = DesignExtractor();
		de.process();

		AffectsCalculator::getInstance()->clear();
		AffectsCalculator *ac = AffectsCalculator::getInstance();

		StmtSet actualSet = pkb->getStmtsByStmt(AFFECTS_STAR, 8);
		StmtSet expectedSet;
		expectedSet.clear();
		for (int i = 4; i <= 8; i++) {
			expectedSet.insert(i);
		}
		expectedSet.insert(9);

		char c[10];
		Logger::WriteMessage("=======");
		for (StmtNumber s : actualSet) Logger::WriteMessage(itoa(s, c, 10));
		Logger::WriteMessage("=======");

		Assert::IsTrue(actualSet == expectedSet);

		pkb->clear();
		ac->clear();
	}
	};
}
