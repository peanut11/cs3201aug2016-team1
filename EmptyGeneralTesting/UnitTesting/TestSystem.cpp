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
			frontend.parse("Iteration_1/source1.txt");

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

		TEST_METHOD(TestSystem_Iteration2_Program1_Example2) {
			// procedure p, q;
			// Select p such that Calls(p, q)
			PKB::getInstance()->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("Iteration_2_Test_Program_Case/source.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();

			std::string strTitle = "TEST VARIABLES TYPE from PKB";
			Logger::WriteMessage(strTitle.c_str());
			std::set<ProcIndex> procedures1 = pkb->getAllProcIndex();
			std::set<ProcIndex> procedures2 = pkb->getAllProcIndex();
			std::set<ProcName> evaluatedProcedures;
			for (std::set<ProcIndex>::iterator it = procedures1.begin(); it != procedures1.end(); ++it) {
				for (std::set<ProcIndex>::iterator itz = procedures2.begin(); itz != procedures2.end(); ++itz) {
					if (pkb->is(CALLS_STAR, *it, *itz)) {
						evaluatedProcedures.insert(pkb->getProcName(*itz));
					}
				}
			}
			for (std::set<ProcName>::iterator it = evaluatedProcedures.begin(); it != evaluatedProcedures.end(); ++it) {
				Logger::WriteMessage((*it).c_str());
			}

			std::string strTitle1 = "TEST EVALUATOR";
			Logger::WriteMessage(strTitle1.c_str());
			std::string declaration2 = "procedure p, q;\n";
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select q such that Calls(p, q)");
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
		}

		TEST_METHOD(TestSystem_Iteration2_Program1_Example3) {
			// Select s1 such that Modifies(s1, "x")
			PKB::getInstance()->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("Iteration_2_Test_Program_Case/source.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();

			std::set<StmtNumber> correctValues = { 65,1,137,9,10,11,13,78,14,79,15,16,82,18,19,35,37,38,110,46,47,48,114,50,51,117,53,55,56,57,123,59,60,125,61,128,64,130,66,131,67,69,70,71,76,81,89,154,90,92,93,158,94,103,105,106,107,108,109,118,122,127,132,136,151,159,163,165,175,179,180,181,190,191,195,199,200,208,210,211,212,213,217,219,220,221,226,227,228,229,232,233,234,235,236,240,241,242,243,244 };
			std::string strTitle = "TEST VARIABLES TYPE from PKB";
			Logger::WriteMessage(strTitle.c_str());
			std::set<StmtNumber> haha2 = pkb->getStmtsByVar(MODIFIES, pkb->getVarIndex("x"));
			for (std::set<StmtNumber>::iterator it = haha2.begin(); it != haha2.end(); ++it) {
				Logger::WriteMessage(std::to_string(*it).c_str());
			}
			Assert::AreEqual(correctValues.size(), haha2.size());

			std::string strTitle1 = "TEST EVALUATOR";
			Logger::WriteMessage(strTitle1.c_str());
			std::string declaration2 = "stmt s1;\n"; 
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select s1 such that Modifies(s1, \"x\")");
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
		}

		TEST_METHOD(TestSystem_Iteration2_Program1_Example4) {
			// procedure p; variable v1;
			// Select p such that Modifies(p, v1)
			PKB::getInstance()->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("Iteration_2_Test_Program_Case/source.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();

			std::string strTitle = "TEST VARIABLES TYPE from PKB";
			Logger::WriteMessage(strTitle.c_str());
			std::set<ProcIndex> procedures1 = pkb->getAllProcIndex();
			std::set<VarIndex> variables = pkb->getAllVarIndex();
			std::set<ProcName> evaluatedProcedures;
			for (std::set<ProcIndex>::iterator proc = procedures1.begin(); proc != procedures1.end(); ++proc) {
				for (std::set<VarIndex>::iterator var = variables.begin(); var != variables.end(); ++var) {
					if (pkb->is(MODIFIES_P, *proc, *var)) {
						evaluatedProcedures.insert(pkb->getProcName(*proc));
					}
				}
			}
			for (std::set<ProcName>::iterator it = evaluatedProcedures.begin(); it != evaluatedProcedures.end(); ++it) {
				Logger::WriteMessage((*it).c_str());
			}

			std::string strTitle1 = "TEST EVALUATOR";
			Logger::WriteMessage(strTitle1.c_str());
			std::string declaration2 = "procedure p; variable v1;\n";
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select p such that Modifies(p, v1)");
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
		}

		TEST_METHOD(TestSystem_Iteration2_Program1_Example9) {
			// if if1, if2; while w;
			// Select if1 such that Next(if1, w) and Next*(if1, if2) pattern if1("y",_, _) and w(_, _)
			PKB::getInstance()->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("Iteration_2_Test_Program_Case/source.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();

			std::string strTitle1 = "TEST EVALUATOR";
			Logger::WriteMessage(strTitle1.c_str());
			std::string declaration2 = "if if1, if2; while w;\n";
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select if1 such that Next(if1, w) and Next*(if1, if2) pattern if1(\"y\",_,_) and w(_,_)");
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
		}

		TEST_METHOD(TestSystem_Iteration2_Program1_Example10) {
			// if if1, if2; while w;
			// Select if1 such that Next(if1, w) and Next*(if1, if2) pattern if1("y",_, _) and w(_, _)
			PKB::getInstance()->clear();

			Frontend frontend = Frontend();
			frontend.parse(std::string("Iteration_2_Test_Program_Case/source.txt"));

			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			PKB* pkb = PKB::getInstance();

			std::string strTitle1 = "TestSystem_Iteration2_Program1_Example10";
			Logger::WriteMessage(strTitle1.c_str());
			std::string declaration2 = "assign a; while w;\n";
			std::vector<std::string> results2 = queryProcessor->evaluate(declaration2 + "Select a such that Next*(a,w) pattern w(_,_)");
			for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); it++) {
				Logger::WriteMessage((*it).c_str());
			}
		}

        TEST_METHOD(TestSystem_Iteration2_Query40) {
            // This query takes ~ 1 min to evaluate
            return; // Comment-out this line to run

            PKB::getInstance()->clear();

            Frontend frontend = Frontend();
            frontend.parse("Iteration_2_Test_Program_Case/source.txt");

            std::string declaration = "procedure p; assign a; variable v1, v2;\n";
            std::string clauses = (std::string("")
                                   + "Select p such that Uses(p, v1) and Uses(a, v1) "
                                   + "and Modifies(p, v2) with v2.varName = \"x\"");

            QueryProcessor *qp = QueryProcessor::getInstance();
            std::vector<std::string> v = qp->evaluate(declaration + clauses);
            std::set<std::string> results(v.begin(), v.end());

            std::set<std::string> expected = { "IamCrazy","Main","Panda","Elephant","Monkey",
                "Giraffe","Dream","First","Second","A","D","Movie","Happy","Useless" };
            Assert::AreEqual(expected.size(), results.size());

            std::set<std::string>::const_iterator expect = expected.begin();
            std::set<std::string>::const_iterator actual = results.begin();
            while (expect != expected.end()) {
                Assert::AreEqual(*expect, *actual);
                expect++;
                actual++;
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
