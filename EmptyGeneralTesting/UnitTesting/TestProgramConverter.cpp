#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ProgramConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestProgramConverter) {

		/*TEST_METHOD_INITIALIZE(initialize) {

		};*/

		TEST_METHOD_CLEANUP(destruct) {
			PKB* pkb = PKB::getInstance();
			pkb->clear();
		};

public:
	TEST_METHOD(TestConvert_ProcSecond) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		int progLines = pc.convert(str);
		Assert::AreEqual(5, progLines);

	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckControlVars) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("prototype_procedure_Second.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		StmtNumber stmt1 = 1;
		StmtNumber stmt2 = 2;
		StmtNumber stmt3 = 3;
		StmtNumber stmt4 = 4;
		StmtNumber stmt5 = 5;
		StmtNumber stmt6 = 6;
		VarName varName = "i";
		VarIndex varIndex = pkb->getVarIndex(varName);
		std::vector<StmtNumber> stmts({ stmt1, stmt2, stmt3, stmt4, stmt5, stmt6 });
		std::vector<bool> expectedWhile({ false, false, true, false, false, false });
		std::vector<bool> expectedIf({ false, false, false, false, false, false });
		for (unsigned int i = 0; i < stmts.size(); i++) {
			Assert::IsTrue(expectedWhile[i] == pkb->isWhilePattern(stmts[i], varIndex));
			Assert::IsTrue(expectedIf[i] == pkb->isIfPattern(stmts[i], varIndex));
		}
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckStmtTypeTable) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("prototype_procedure_ConverterTest3.txt");
		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		std::vector<EntityType> expected;
		expected.push_back(ASSIGN);
		expected.push_back(ASSIGN);
		expected.push_back(WHILE);
		expected.push_back(ASSIGN);
		expected.push_back(WHILE);
		expected.push_back(ASSIGN);
		expected.push_back(ASSIGN);
		expected.push_back(IF);
		expected.push_back(ASSIGN);
		expected.push_back(ASSIGN);
		expected.push_back(ASSIGN);

		Assert::AreEqual(expected.size(), pkb->getStmtTableSize());

		StmtNumber stmt;
		for (unsigned int i = 0; i < expected.size(); i++) {
			stmt = i + 1;
			Assert::IsTrue(expected[i] == pkb->getStmtTypeForStmt(stmt));
		}
		

	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckStmtByTypeTable) {

		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest3.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		StmtNumber stmt1 = 1;
		StmtNumber stmt2 = 2;
		StmtNumber stmt3 = 3;
		StmtNumber stmt4 = 4;
		StmtNumber stmt5 = 5;
		StmtNumber stmt6 = 6;
		StmtNumber stmt7 = 7;
		StmtNumber stmt8 = 8;
		StmtNumber stmt9 = 9;
		StmtNumber stmt10 = 10;
		StmtNumber stmt11 = 11;

		// type == STMT
		std::set<StmtNumber> expectedStmts({stmt1, stmt2, stmt3, stmt4, stmt5, stmt6, stmt7, stmt8, stmt9,stmt10, stmt11});
		std::set<StmtNumber> actual = pkb->getStmtsByType(STMT);
		std::set<StmtNumber>::const_iterator expectedIt = expectedStmts.begin();
		std::set<StmtNumber>::const_iterator actualIt = actual.begin();
		Assert::AreEqual(expectedStmts.size(), actual.size());
		while (expectedIt != expectedStmts.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// type == ASSIGN
		expectedStmts = std::set<StmtNumber> ({ stmt1, stmt2, stmt4, stmt6, stmt7, stmt9, stmt10, stmt11 });
		actual = pkb->getStmtsByType(ASSIGN);
		expectedIt = expectedStmts.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expectedStmts.size(), actual.size());
		while (expectedIt != expectedStmts.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// type == WHILE
		expectedStmts = std::set<StmtNumber>({ stmt3, stmt5 });
		actual = pkb->getStmtsByType(WHILE);
		expectedIt = expectedStmts.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expectedStmts.size(), actual.size());
		while (expectedIt != expectedStmts.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// type == STMTLST
		expectedStmts = std::set<StmtNumber>({ stmt1, stmt4, stmt6, stmt9, stmt10});
		actual = pkb->getStmtsByType(STMTLST);
		expectedIt = expectedStmts.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expectedStmts.size(), actual.size());
		while (expectedIt != expectedStmts.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// type == IF
		expectedStmts = std::set<StmtNumber>({ stmt8 });
		actual = pkb->getStmtsByType(IF);
		expectedIt = expectedStmts.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expectedStmts.size(), actual.size());
		while (expectedIt != expectedStmts.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}
	}

	TEST_METHOD(TestUpdateStmtInStmtTable_CheckStmtTable) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("prototype_procedure_ConverterTest3.txt");

		ProgramConverter pc = ProgramConverter();
		Assert::AreEqual(11 , pc.convert(str));
		StmtNumber stmt2 = 2;
		StmtNumber stmt3 = 3;
		StmtNumber stmt7 = 7;
		StmtNumber stmt8 = 8;
		StmtNumber stmt9 = 9;
		StmtNumber stmt10 = 10;
		StmtNumber stmt11 = 11;

		// check parent of line in else
		std::set<StmtNumber> expected({ stmt8 });
		std::set<StmtNumber> actual = pkb->getStmtsByStmt(stmt10, PARENT);
		std::set<StmtNumber>::const_iterator expectedIt = expected.begin();
		std::set<StmtNumber>::const_iterator actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check children of if stmts
		expected = std::set<StmtNumber>({ stmt9, stmt10 });
		actual = pkb->getStmtsByStmt(PARENT, stmt8);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check followers of if stmts
		expected = std::set<StmtNumber>({ stmt11 });
		actual = pkb->getStmtsByStmt(FOLLOWS, stmt8);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check next of if stmts
		expected = std::set<StmtNumber>({ stmt9, stmt10 });
		actual = pkb->getStmtsByStmt(NEXT, stmt8);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check previous of else stmts
		expected = std::set<StmtNumber>({ stmt8 });
		actual = pkb->getStmtsByStmt(stmt10, NEXT);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check previous of after else stmts
		expected = std::set<StmtNumber>({ stmt9, stmt10 });
		actual = pkb->getStmtsByStmt(stmt11, NEXT);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check previous of while stmts
		expected = std::set<StmtNumber>({ stmt2, stmt7 });
		actual = pkb->getStmtsByStmt(stmt3, NEXT);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}
	}
	TEST_METHOD(TestUpdateAssignmentInTable_CheckVarRefTable) {
		PKB* pkb = PKB::getInstance();
		std::string str = Tools::readFile("prototype_procedure_ConverterTest1.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		std::set<VarName> expected;
		expected.insert("x");
		expected.insert("i");
		expected.insert("z");
		expected.insert("y");
		std::set<VarName> actual;
		actual = pkb->getAllVarNames();


		Assert::IsTrue(expected == actual);

	}

	TEST_METHOD(TestConvert_CheckVarRefMap) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest1.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		VarName varName1 = "x";
		VarIndex varIndex1 = 0;
		VarName varName2 = "i";
		VarIndex varIndex2 = 1;
		VarName varName3 = "z";
		VarIndex varIndex3 = 2;
		VarName varName4 = "y";
		VarIndex varIndex4 = 3;
		VarRefMap expected = std::map<ProcName, ProcIndex>({ { varName1, varIndex1 }, { varName2, varIndex2 }, { varName3, varIndex3 }, { varName4, varIndex4 } });

		std::set<VarName> actual = pkb->getAllVarNames(); //get from VarRefTable

		Assert::AreEqual(expected.size(), actual.size());

		VarRefMap::const_iterator expectedIt = expected.begin();
		std::set<VarName>::const_iterator actualIt = actual.begin();

		while (expectedIt != expected.end()) {
			Assert::AreEqual(expectedIt->second, pkb->getVarIndex(*actualIt));

			expectedIt++;
			actualIt++;
		}

	}

	TEST_METHOD(TestConvert_CheckVarTable) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest1.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		// check procedures
		VarName varName1 = "x";
		VarIndex varIndex1 = pkb->getVarIndex(varName1);

		ProcName procName = "Second";
		ProcIndex procIndex = pkb->getProcIndex(procName);

		// check procedures that directly modifies x
		std::set<ProcIndex> expected({ procIndex });
		std::set<ProcIndex> actual= pkb->getProcsByVar(MODIFIES, varIndex1);
		std::set<ProcIndex>::const_iterator expectedIt = expected.begin();
		std::set<ProcIndex>::const_iterator actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check procedures that directly uses x
		expected = std::set<ProcIndex>({ procIndex });
		actual = pkb->getProcsByVar(USES, varIndex1);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check stmts that directly modifies x
		StmtNumber stmt1 = 1;
		StmtNumber stmt6 = 6;
		expected = std::set<StmtNumber>({ stmt1, stmt6 });
		actual = pkb->getStmtsByVar(MODIFIES, varIndex1);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

		// check stmts that directly uses x
		StmtNumber stmt4 = 4;
		expected = std::set<StmtNumber>({ stmt4});
		actual = pkb->getStmtsByVar(USES, varIndex1);
		expectedIt = expected.begin();
		actualIt = actual.begin();
		Assert::AreEqual(expected.size(), actual.size());
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}
	}
	TEST_METHOD(TestUpdateAssignmentInTable_CheckConstants) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest1.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		Constant const1 = 0;
		Constant const2 = 5;
		Constant const3 = 2;
		Constant const4 = 7;

		std::set<Constant> expected({ const1, const2, const3, const4 });
		std::set<Constant> actual = pkb->getAllConstantValues();

		Assert::AreEqual(expected.size(), actual.size());

		std::set<Constant>::const_iterator expectedIt = expected.begin();
		std::set<Constant>::const_iterator actualIt = actual.begin();

		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

	}

	TEST_METHOD(TestConvert_CheckProcRefTable) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest1.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		ProcName proc = "Second";

		std::set<ProcName> expected({ proc });
		std::set<ProcName> actual = pkb->getAllProcNames();

		Assert::AreEqual(expected.size(), actual.size());

		std::set<ProcName>::const_iterator expectedIt = expected.begin();
		std::set<ProcName>::const_iterator actualIt = actual.begin();

		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

	}

	TEST_METHOD(TestConvert_CheckProcRefMap) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest1.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		ProcName procName = "Second";
		ProcIndex procIndex = 0;
		ProcRefMap expected = std::map<ProcName, ProcIndex>({ { procName, procIndex } });

		std::set<ProcName> actual = pkb->getAllProcNames(); //get from ProcRefTable

		Assert::AreEqual(expected.size(), actual.size());

		ProcRefMap::const_iterator expectedIt = expected.begin();
		std::set<ProcName>::const_iterator actualIt = actual.begin();

		while (expectedIt != expected.end()) {
			Assert::AreEqual(expectedIt->second, pkb->getProcIndex(*actualIt));

			expectedIt++;
			actualIt++;
		}

	}

	TEST_METHOD(TestConvert_CheckProcToStmtTable) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest1.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		ProcName procName = "Second";
		ProcIndex procIndex = pkb->getProcIndex(procName);
		StmtNumber stmt1 = 1;
		StmtNumber stmt2 = 2;
		StmtNumber stmt3 = 3;
		StmtNumber stmt4 = 4;
		StmtNumber stmt5 = 5;
		StmtNumber stmt6 = 6;
		std::set<StmtNumber> expected({ stmt1, stmt2, stmt3, stmt4, stmt5, stmt6 });

		std::set<StmtNumber> actual = pkb->getStmtsByProc(procIndex);

		Assert::AreEqual(expected.size(), actual.size());

		std::set<StmtNumber>::const_iterator expectedIt = expected.begin();
		std::set<StmtNumber>::const_iterator actualIt = actual.begin();

		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);

			expectedIt++;
			actualIt++;
		}

	}

	TEST_METHOD(TestConvert_CheckStmtToProcTable) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest2.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);
		ProcName procName1 = "Second";
		ProcName procName2 = "Third";
		ProcIndex procIndex1 = pkb->getProcIndex(procName1);
		ProcIndex procIndex2 = pkb->getProcIndex(procName2);
		StmtNumber stmt1 = 1;
		StmtNumber stmt2 = 2;
		StmtNumber stmt3 = 3;
		StmtNumber stmt4 = 4;
		StmtNumber stmt5 = 5;
		StmtNumber stmt6 = 6;
		StmtNumber stmt7 = 7;
		StmtNumber stmt8 = 8;
		std::set<StmtNumber> stmts({ stmt1, stmt2, stmt3, stmt4, stmt5, stmt6, stmt7, stmt8 });

		std::set<StmtNumber>::const_iterator stmtsIt = stmts.begin();

		while (stmtsIt != stmts.end()) {
			if (*stmtsIt <= stmt7) {
				Assert::AreEqual(procIndex1, pkb->getProcByStmt(*stmtsIt));
			}
			else {
				Assert::AreEqual(procIndex2, pkb->getProcByStmt(*stmtsIt));
			}
			stmtsIt++;
		}

	}

	TEST_METHOD(TestConvert_CheckProcTable) {
		PKB* pkb = PKB::getInstance();

		std::string str = Tools::readFile("prototype_procedure_ConverterTest2.txt");

		ProgramConverter pc = ProgramConverter();
		pc.convert(str);

		// check direct modifies for Third
		ProcName procName = "Third";
		ProcIndex procIndex = pkb->getProcIndex(procName);
		VarName varName = "y";
		VarIndex varIndex = pkb->getVarIndex(varName);
		std::set<ProcStmtVarIndex> expected({ varIndex });
		std::set<ProcStmtVarIndex> actual = pkb->getVarsByProc(procIndex, MODIFIES);
		Assert::AreEqual(expected.size(), actual.size());

		std::set<ProcStmtVarIndex>::const_iterator expectedIt = expected.begin();
		std::set<ProcStmtVarIndex>::const_iterator actualIt = actual.begin();
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);
			expectedIt++;
			actualIt++;
		}

		//check direct uses for Third
		procName = "Third";
		varName = "y";
		varIndex = pkb->getVarIndex(varName);
		expected = std::set<VarIndex>({ varIndex });
		actual = pkb->getVarsByProc(procIndex, USES);
		Assert::AreEqual(expected.size(), actual.size());

		expectedIt = expected.begin();
		actualIt = actual.begin();
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);
			expectedIt++;
			actualIt++;
		}

		// check direct calls for Second
		ProcName procName1 = "Second";
		ProcIndex procIndex1 = pkb->getProcIndex(procName1);
		ProcName procName2 = "Third";
		procIndex = pkb->getProcIndex(procName2);
		expected = std::set<ProcStmtVarIndex>({ procIndex });
		actual = pkb->getProcsByProc(procIndex1, CALLS);
		Assert::AreEqual(expected.size(), actual.size());

		expectedIt = expected.begin();
		actualIt = actual.begin();
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);
			expectedIt++;
			actualIt++;
		}

		// check direct called_by for Third
		procName1 = "Second";
		procName2 = "Third";
		ProcIndex procIndex2 = pkb->getProcIndex(procName2);
		procIndex = pkb->getProcIndex(procName1);
		expected = std::set<ProcStmtVarIndex>({ procIndex });
		actual = pkb->getProcsByProc(CALLS, procIndex2);
		Assert::AreEqual(expected.size(), actual.size());

		expectedIt = expected.begin();
		actualIt = actual.begin();
		while (expectedIt != expected.end()) {
			Assert::AreEqual(*expectedIt, *actualIt);
			expectedIt++;
			actualIt++;
		}
	}
	};
}

