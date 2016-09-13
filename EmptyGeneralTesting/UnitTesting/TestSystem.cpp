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
			Frontend frontend = Frontend();
			frontend.parse(std::string("Source1_Basic.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();
			std::set<StmtNumber> haha = pkb->getStmtsByStmt(2, FOLLOWS);
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

			std::string declaration = "stmt s;\n";

			Assert::IsTrue(queryProcessor->getQueryPreProcessor()->isValidQuery(declaration + "Select s such that Follows(s,2)"));
//			Logger::WriteMessage(queryProcessor->getQueryPreProcessor()->getQueryTable().toString().c_str());
			
//			Logger::WriteMessage(queryProcessor->getQueryPreProcessor()->getSynonymTable()->toString().c_str());
			strTitle = "GET STATEMENT BY STATEMENT from PKB";
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
			Assert::AreEqual("1", std::to_string(results.size()).c_str());
		}
	};
}