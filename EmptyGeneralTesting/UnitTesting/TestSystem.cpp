#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/StringTokenizer.h"
#include "../SPA/QueryProcessor.h"
#include "../SPA/QueryPreProcessor.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/RelTable.h"
#include "../SPA/RelObject.h"
#include "../SPA/RelationshipType.h"
#include "../SPA/EntityType.h"
#include "../SPA/Frontend.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestSystem)
	{
	public:

		TEST_METHOD(TestSystem_Full) {
            PKB::getInstance()->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("it1/source1.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();

			std::string dec = "assign a; variable v;\n";
			try {
				std::vector<std::string> res = queryProcessor->evaluate(dec + "Select v such that Parent(3, a) pattern a(v,_\"0\"_)");
			}
			catch (size_t i) {
				Assert::IsTrue(i == 1);
				//Assert::IsTrue(res.size() == 0);
			}

/*			std::set<StmtNumber> haha = pkb->getStmtsByStmt(2, FOLLOWS);
			for (std::set<StmtNumber>::iterator it = haha.begin(); it != haha.end(); ++it) {
//				Logger::WriteMessage(std::to_string(*it).c_str());
			}

			std::string strTitle = "TEST ENTITY TYPE from PKB";
			Logger::WriteMessage(strTitle.c_str());
			std::set<StmtNumber> haha2 = pkb->getStmtsByType(STMT);
			for (std::set<StmtNumber>::iterator it = haha2.begin(); it != haha2.end(); ++it) {
                Logger::WriteMessage(std::to_string(*it).c_str());
			}

			strTitle = "TEST ENTITY TYPE from PKB (varNames)";
			Logger::WriteMessage(strTitle.c_str());
			std::set<VarName> haha3 = pkb->getAllVarNames();
			for (std::set<VarName>::iterator it = haha3.begin(); it != haha3.end(); ++it) {
				Logger::WriteMessage((*it).c_str());
			}

			/*
			std::set<StmtNumber> b = queryProcessor->getQueryEvaluator()->getPKB()->getStmtsByStmt(2, FOLLOWS);
			for (std::set<StmtNumber>::iterator it = b.begin(); it != b.end(); ++it) {
//				Logger::WriteMessage(std::to_string(*it).c_str());
			}
			*/

/*			std::string declaration = "stmt s;\n";
			Assert::IsTrue(queryProcessor->getQueryPreProcessor()->isValidQuery(declaration + "Select s such that Follows(s,2)"));

//			Logger::WriteMessage(queryProcessor->getQueryPreProcessor()->getQueryTable().toString().c_str());
//			Logger::WriteMessage(queryProcessor->getQueryPreProcessor()->getSynonymTable()->toString().c_str());

/*			strTitle = "GET STATEMENT BY STATEMENT from PKB";
			Logger::WriteMessage(strTitle.c_str());
			std::set<StmtNumber> c = queryProcessor->getQueryEvaluator()->getPKB()->getStmtsByStmt(2, FOLLOWS);
			for (std::set<StmtNumber>::iterator it = c.begin(); it != c.end(); ++it) {
				Logger::WriteMessage(std::to_string(*it).c_str());
			}
			std::vector<std::string> results = queryProcessor->evaluate(declaration + "Select s such that Follows(s,2)");
			Logger::WriteMessage(queryProcessor->getQueryEvaluator()->getResultsTable().toString().c_str());
			for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
			Assert::AreEqual(size_t(1), results.size());

			std::string declaration1 = "stmt s;\n";
			std::vector<std::string> results1 = queryProcessor->evaluate(declaration + "Select s such that Parent(3,s)");
			Logger::WriteMessage(queryProcessor->getQueryEvaluator()->getResultsTable().toString().c_str());
			for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
			Assert::AreEqual(size_t(2), results1.size());
*/
/*			strTitle = "GET STATEMENT BY ASSIGN from PKB";
			Logger::WriteMessage(strTitle.c_str());
            std::set<StmtNumber> c1 = pkb->getStmtsByType(ASSIGN);
			for (std::set<StmtNumber>::iterator it = c1.begin(); it != c1.end(); ++it) {
				Logger::WriteMessage(std::to_string(*it).c_str());
			} */

/*			std::string declaration2 = "assign a;\n";
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select a pattern a(_,_\"z\"_)");
			Logger::WriteMessage(queryProcessor->getQueryPreProcessor()->getQueryTable().toString().c_str());
			Logger::WriteMessage(queryProcessor->getQueryEvaluator()->getResultsTable().toString().c_str());
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
			Assert::AreEqual(size_t(2), results2.size()); */

/*			std::string declaration2 = "stmt s;\n";
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select s such that Follows*(2, s)");
			Logger::WriteMessage(queryProcessor->getQueryPreProcessor()->getQueryTable().toString().c_str());
			// Logger::WriteMessage(queryProcessor->getQueryEvaluator()->getResultsTable().toString().c_str());
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
			Assert::AreEqual(size_t(1), results2.size()); // Follows*(2, 3) only
			*/
		}
		
		TEST_METHOD(TestSystem_Iteration2_Program1_Example1) {
			// Select v such that Modifies (24, v) 
			PKB::getInstance()->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("Iteration_2_Test_Program_Case/Source_1.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();

			std::string strTitle = "TEST VARIABLES TYPE from PKB";
			Logger::WriteMessage(strTitle.c_str());
			std::set<VarIndex> haha2 = pkb->getVarsByStmt(24, MODIFIES);
			for (std::set<VarIndex>::iterator it = haha2.begin(); it != haha2.end(); ++it) {
				Logger::WriteMessage(pkb->getVarName(*it).c_str());
			}

			std::string strTitle1 = "TEST EVALUATOR";
			Logger::WriteMessage(strTitle1.c_str());
			std::string declaration2 = "variable v;\n";
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select v such that Modifies(24,v)");
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
		}

        TEST_METHOD(TestSystem_NestedIfWhileIf) {
            PKB::getInstance()->clear();

            Frontend frontend = Frontend();
            frontend.parse(std::string("procedure_NestedIfWhileIf.txt"));
        }

        TEST_METHOD(TestSystem_NestedIfIf) {
            PKB::getInstance()->clear();

            Frontend frontend = Frontend();
            frontend.parse(std::string("procedure_NestedIfIf.txt"));
        }
	};
}
