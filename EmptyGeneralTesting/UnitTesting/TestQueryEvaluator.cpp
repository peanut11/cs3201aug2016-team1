#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/AttrType.h"
#include "../SPA/ClauseSelectObject.h"
#include <iostream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryEvaluator) {

public:
    TEST_METHOD(TestQueryEvaluator_SelectObject) {
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        // 'Select BOOLEAN'
		ClauseSelectObject selTrue = ClauseSelectObject(CONSTANT, "", AttrType::VAR_NAME, true);
        // 'Select s'
        ClauseSelectObject selFalse = ClauseSelectObject(CONSTANT, "s", AttrType::VAR_NAME, false);
        // Relationships holds
        Assert::AreEqual(std::string("true"), evaluator->evaluateSelect(selTrue, true)[0]);
        // Relationships don't holds
        Assert::AreEqual(std::string("false"), evaluator->evaluateSelect(selTrue, false)[0]);
        int resultsSize = evaluator->evaluateSelect(selFalse, false).size();
        // Assert::AreEqual(0, resultsSize);
    }

    TEST_METHOD(TestQueryEvaluator_SetIntersectionNumber) {
        std::set<StmtNumber> currentStatements = { 1,2,3,4,5,6,7,8,9 };
        std::set<StmtNumber> evaluatedS = { 6,7 };
        std::set<StmtNumber> updatedStatements;
        set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
                         std::inserter(updatedStatements, updatedStatements.begin()));

        std::set<StmtNumber> current = updatedStatements;
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(current.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_SetIntersectionString) {
        std::set<VarName> currentStatements = { "a", "b", "c", "d" };
        std::set<VarName> evaluatedS = { "b", "c" };
        std::set<VarName> updatedStatements;
        set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
                         std::inserter(updatedStatements, updatedStatements.begin()));

        std::set<VarName> current = updatedStatements;
        for (std::set<VarName>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(current.size()).c_str());

    }

    /*
    TEST_METHOD(TestQueryEvaluator_PopulateResultsTable) {

        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        // assign a, while w, variable v
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        SynonymObject a(ASSIGN, "a");
        SynonymObject w(WHILE, "w");
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(a);
        synonymTable->insert(w);
        synonymTable->insert(v);

        ResultGridManager* resultManager = evaluator->populateResultGrids();
        Assert::AreEqual(std::string("a"), resultsTable->getObject("a")->getSynonym());
        Assert::AreEqual(std::string("w"), resultsTable->getObject("w")->getSynonym());
        Assert::AreEqual(std::string("v"), resultsTable->getObject("v")->getSynonym());
        Assert::AreNotEqual(std::string("z"), resultsTable->getObject("v")->getSynonym());
        Logger::WriteMessage(std::to_string(resultManager->getValuesForSynonym("a").size()).c_str());
    }
    */

	// FOLLOWS (4,5)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow1) {
		// Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// FOLLOWS (4,5) <--- True
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
        Logger::WriteMessage(std::to_string(argOne->getIntegerValue()).c_str());
        Logger::WriteMessage(std::to_string(argTwo->getIntegerValue()).c_str());
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());
		Logger::WriteMessage("=============");

		// FOLLOWS (1,4) <--- False
        ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(STMT, std::string(""), 1, false);
        ClauseSuchThatArgObject* arg4 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
        ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, arg3, arg4);
        ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1);
        Assert::IsFalse(re1->getResultsBoolean());

        // SELECT BOOLEAN such that Follows(4,5)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());

        // SELECT s such that Follows(4,5)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
    }

	// FOLLOWS (s,10)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow2) {
		// Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// FOLLOWS (s,10) <--- True
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string(""), 10, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "s"
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s,10)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s,10)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// FOLLOWS (a,8)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow3) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (a,8)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(ASSIGN, std::string("a"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string(""), 8, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated statements
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(a,8)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Follows(a,8)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
    }

	//  FOLLOWS (10,s)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow4) {
        //  Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (10,s)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 10, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "s"
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(10,s)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(10,s)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
    }
	
	//  FOLLOWS (16,a)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow5) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (16,a)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 16, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(ASSIGN, std::string("a"), 0, true);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "a"
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(16,a)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Follows(16,a)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
    }
	
	//  FOLLOWS (_,10)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow6) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (_,10)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string(""), 10, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(_,10);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(_,10);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
    }

	// FOLLOWS (_,s)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow7) {
        //  Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (_,s)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = {2, 3, 5, 6, 8, 9, 10, 13, 14, 15, 17}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(_,s);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(_,s);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(results1.size()).c_str());
    }

	// FOLLOWS (8,_)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow8) {
		// Initialization 
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (8,_)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 8, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

        // SELECT BOOLEAN such that Follows(8,_);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(8,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// FOLLOWS (s,_)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow9) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (s,_)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = { 1,2,4,5,6,7,8,10,13,14,16 }
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s,_);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// FOLLOWS (s1, s2) - BUGGED
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow10) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s1(STMT, "s1");
        SynonymObject s2(STMT, "s2");
        SynonymObject s(STMT, "s");
        synonymTable->insert(s1);
        synonymTable->insert(s2);
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s1");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");
        std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s2");
        for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current1.size()).c_str());
        Logger::WriteMessage("=============");

		// FOLLOWS (s1,s2)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s1 = {1,2,4,5,6,7,8,10,13,14,16}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s1");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

		// s2 = {2,3,5,6,8,9,10,13,14,15,17}
        std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("s2");
        for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s1,s2)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());

        Logger::WriteMessage("=============");

        // SELECT s1 such that Follows(s1,s2)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(results1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT s2 such that Follows(s1,s2)
        ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(results2.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s1,s2)
        ClauseSelectObject selObject3 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results3 = evaluator->evaluateSelect(selObject3, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results3.size()).c_str());
    }

	// MODIFIES (4, "x")
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies1) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// MODIFIES (4, "x") --> TRUE
        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// MODIFIES (3, "y") --> FALSE
        ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(STMT, std::string("y"), 0, false);
        ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, argOne, arg3);
        ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1);
        Assert::IsFalse(re1->getResultsBoolean());

        // SELECT BOOLEAN such that (Modifies(4,"x"))
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(4,"x"))
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
    }

	// MODIFIES (9,v)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies2) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print variables
        std::set<VarName> current = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// MODIFIES (9,v)
        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 9, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "v"
        std::set<VarName> updated = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(9,v))
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that (Modifies(9,v))
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// MODIFIES (13,_)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies3) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print variables
        std::set<VarName> current = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

		// MODIFIES (13,_)
        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 13, false);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("_"), 0, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(13,_)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that (Modifies(13,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// MODIFIES (s,"x")
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies4) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// MODIFIES (s,"x")
        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = {1,4,7,11,15}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(s,"x")
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(s,"x")
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// MODIFIES (s,_)
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies5) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// MODIFIES (s,_)
        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = {1,2,4,5,7,9,11,12,13,14,15,16,17}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(s,_);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(s,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// MODIFIES (s,v) - BUGGED
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies6) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// Print variables
        std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

		// MODIFIES (s,v)
        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = {1,2,4,5,7,9,11,12,13,14,15,16,17}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(updated.size()).c_str());

		// v = { i, x, y, z }
        std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Modifies(s,v);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Modifies(s,v);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT v such that Modifies(s,v);
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results2.size()).c_str());
    }
	
	// PATTERN a(v,_) - NOT YET INITIALIZED
    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern1) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// Print variables
        std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

		// PATTERN a(v,_)
        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "v";
        std::string secondArg = "_";
        bool isFirstArgSynonym = true;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = {1,2,4,5,7,9,11,12,13,14,15,16,17}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(updated.size()).c_str());

		// v = {i,x,y,z}
        std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Pattern a(v,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(v,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that Pattern a(v,_)
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results2.size()).c_str());
    }

	// PATTERN a(_,_) - NOT YET INITIALIZED
    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern2) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// PATTERN a(_,_)
        EntityType patternType = ASSIGN;
        EntityType firstArgType = WILDCARD;
        std::string patternSynonymArg = "a";
        std::string firstArg = "_";
        std::string secondArg = "_";
        bool isFirstArgSynonym = false;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

        // SELECT BOOLEAN such that Pattern a(_,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(_,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
	
	// PATTERN a("x",_) - NOT YET INITIALIZED
    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern3) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// PATTERN a("x",_)
        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "x";
        std::string secondArg = "_";
        bool isFirstArgSynonym = false;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = {1,4,7,11,15}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(6).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that Pattern a("x",_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a("x",_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(6).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
	
	// PATTERN a(v,"_x_") - NOT YET INITIALIZED
    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern4) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// Print variables
        std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

		// PATTERN a(v,"_x_")
        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "v";
        std::string secondArg = "_x_";
        bool isFirstArgSynonym = true;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = { 7,11,13 }
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

		// v = { x,z }
        std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Pattern a(v,"_x_")
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(v,"_x_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that Pattern a(v,"_x_")
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());
    }

	// PATTERN a(_, "_x_") - NOT YET INITIALIZED
    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern5) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

		// PATTERN a(_, "_x_")
        EntityType patternType = ASSIGN;
        EntityType firstArgType = INVALID;
        std::string patternSynonymArg = "a";
        std::string firstArg = "_";
        std::string secondArg = "_x_";
        bool isFirstArgSynonym = false;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = { 7,11,13 }
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that a(_, "_x_")
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(_, "_x_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }

	// PATTERN a("z","_z_") - NOT YET INITIALIZED
    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern6) {
        // Initialization
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
		
		// PATTERN a("z","_z_")
        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "z";
        std::string secondArg = "_z_";
        bool isFirstArgSynonym = false;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = { 13 }
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that Pattern a("z","_z_")
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a("z","_z_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
		
	// CALLS ("First", "Second")
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls1) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// CALLS ("First", "Second") <--- True
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("First"), 0, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("Second"), 0, false);
		Logger::WriteMessage((argOne->getStringValue()).c_str());
		Logger::WriteMessage((argTwo->getStringValue()).c_str());
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());
		Logger::WriteMessage("=============");

		// CALLS ("First", "Third") <--- False
		ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(PROCEDURE, std::string("First"), 0, false);
		ClauseSuchThatArgObject* arg4 = new ClauseSuchThatArgObject(PROCEDURE, std::string("Third"), 0, false);
		ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, arg3, arg4);
		ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1);
		Assert::IsFalse(re1->getResultsBoolean());

		// SELECT BOOLEAN such that CALLS ("First", "Second")
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());

		// SELECT s such that CALLS ("First", "Second")
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// CALLS (p,"Third")
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls2) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print procedures
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator itz = current1.begin(); itz != current1.end(); ++itz) {
			Logger::WriteMessage(std::to_string(*itz).c_str());
		}
		Logger::WriteMessage("=============");

		// CALLS (p,"Third") <--- True
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("Third"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "p"
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (p,"Third")
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (p,"Third")
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	//  CALLS ("First",p)
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls3) {
		//  Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// Print procedures
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator itz = current1.begin(); itz != current1.end(); ++itz) {
			Logger::WriteMessage(std::to_string(*itz).c_str());
		}
		Logger::WriteMessage("=============");

		// CALLS ("First",p)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("First"), 0, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("p"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "p"
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS ("First",p)
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS ("First",p)
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	//  CALLS (_,"Second")
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls4) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// CALLS (_,"Second")
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("Second"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (_,"Second");
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (_,"Second");
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// CALLS (_,p)
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls5) {
		//  Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// Print procedures
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator itz = current1.begin(); itz != current1.end(); ++itz) {
			Logger::WriteMessage(std::to_string(*itz).c_str());
		}
		Logger::WriteMessage("=============");

		// CALLS (_,p)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("_"), 0, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("p"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p = { 1, 2 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (_,p);
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (_,p);
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// CALLS ("First",_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls6) {
		// Initialization 
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// CALLS ("First",_)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("First"), 8, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("_"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// SELECT BOOLEAN such that CALLS ("First",_)
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS ("First",_)
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// CALLS (p,_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls7) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// Print procedures
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator itz = current1.begin(); itz != current1.end(); ++itz) {
			Logger::WriteMessage(std::to_string(*itz).c_str());
		}
		Logger::WriteMessage("=============");

		// CALLS (p,_)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("_"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p = { 0, 1 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (p,_)
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (p,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// CALLS (p1, p2) - BUGGED
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Calls8) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject p1(PROCEDURE, "p1");
		SynonymObject p2(PROCEDURE, "p2");
		SynonymObject s(STMT, "s");
		synonymTable->insert(p1);
		synonymTable->insert(p2);
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("p1");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("p2");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(current1.size()).c_str());
		Logger::WriteMessage("=============");

		// CALLS (p1, p2) 
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p1 = { 0, 1 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p1");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());
		Logger::WriteMessage("=============");

		// p2 = { 1, 2 }
		std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("s2");
		for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (p1, p2) 
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());

		Logger::WriteMessage("=============");

		// SELECT p1 such that CALLS (p1, p2) 
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT p2 such that CALLS (p1, p2) 
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);
		std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (p1, p2) 
		ClauseSelectObject selObject3 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results3 = evaluator->evaluateSelect(selObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results3.size()).c_str());
	}
	
	// MODIFIES ("First", "x")
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_ModifiesProcedure1) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// MODIFIES ("First", "x") --> TRUE
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("First"), 0, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// MODIFIES ("First", "y") --> FALSE
		ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(STMT, std::string("y"), 0, false);
		ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, argOne, arg3);
		ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1);
		Assert::IsFalse(re1->getResultsBoolean());

		// SELECT BOOLEAN such that MODIFIES ("First", "x")
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that MODIFIES ("First", "x")
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// MODIFIES ("First",v)
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_ModifiesProcedure2) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(v);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print variables
		std::set<VarName> current = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// MODIFIES ("First",v)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("First"), 0, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "v"
		std::set<VarName> updated = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that MODIFIES ("First",v)
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that MODIFIES ("First",v)
		ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
	}
	
	// MODIFIES ("Second",_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_ModifiesProcedure3) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(v);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print variables
		std::set<VarName> current = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		// MODIFIES ("Second",_)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("Second"), 0, false);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("_"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that MODIFIES ("Second",_)
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that MODIFIES ("Second",_)
		ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
	}
	
	// MODIFIES (p,"x")
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_ModifiesProcedure4) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print procedures
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator itz = current1.begin(); itz != current1.end(); ++itz) {
			Logger::WriteMessage(std::to_string(*itz).c_str());
		}
		Logger::WriteMessage("=============");

		// MODIFIES (p,"x")
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// s = { 0, 1 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that MODIFIES (p,"x")
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p such that MODIFIES (p,"x")
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
	}
	
	// MODIFIES (p,_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_ModifiesProcedure5) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print procedures
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator itz = current1.begin(); itz != current1.end(); ++itz) {
			Logger::WriteMessage(std::to_string(*itz).c_str());
		}
		Logger::WriteMessage("=============");

		// MODIFIES (p,_)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p = { 0, 1, 2}
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that MODIFIES (p,_)
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p such that MODIFIES (p,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
	}

	// MODIFIES (p,v) - BUGGED
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_ModifiesProcedure6) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(v);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print procedures
		std::set<ProcIndex> current = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print variables
		std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Logger::WriteMessage("=============");

		// MODIFIES (p,v)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p = { 0, 1, 2 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

		// v = { i, v, x, y, z }
		std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN such that MODIFIES (p,v);
		ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
		std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p such that MODIFIES (p,v);
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT v such that MODIFIES (p,v);
		ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results2.size()).c_str());
	}
	
	TEST_METHOD(TestQueryEvaluator_TestEvaluateFollowPattern1) {
        // Follow (a,7) and Pattern a(v,_)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

        // Follow (a,7)
        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(ASSIGN, std::string("a"), 0, true);
        ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string(""), 7, false);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean()); // ERROR: is(FOLLOWS, _, 7) is false in DummyPKB
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        // Pattern a(v,_)
        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "v";
        std::string secondArg = "_";
        bool isFirstArgSynonym = true;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj1 = evaluator->evaluatePattern(patternObj);
        Assert::IsFalse(resultObj1->getResultsBoolean());
        Logger::WriteMessage("====asdasdsad======");

        std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that  Follow (a,8) and Pattern a(v,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean() && resultObj1->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("false", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that  Follow (a,8) and Pattern a(v,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(0).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
	
    TEST_METHOD(TestQueryEvaluator_TestEvaluateModifiesPattern1) {
        // Modifies(8,v) and Pattern a(v,_)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

        // Modifies(8,v) 
        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 8, false);
        ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("v");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        // Pattern a(v,_)
        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "v";
        std::string secondArg = "_";
        bool isFirstArgSynonym = true;
        ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject* resultObj1 = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj1->getResultsBoolean());
        std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that  Modifies(8,v)  and Pattern a(v,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj->getResultsBoolean() && resultObj1->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that  Modifies(8,v)  and Pattern a(v,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
	

	TEST_METHOD(TestQueryEvaluator_Populate_Synonym_Group) {

		// follow the example in slide
		/*
		1:	s3	v1	s1	s2
		2:	a1	v2	a2
		3:	a3	v3
		*/
		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s3", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("s2", 1);

		mSynonymGroup->insertSynonym("a1", 2);
		mSynonymGroup->insertSynonym("v2", 2);
		mSynonymGroup->insertSynonym("a2", 2);

		mSynonymGroup->insertSynonym("a3", 3);
		mSynonymGroup->insertSynonym("v3", 3);

		Logger::WriteMessage(mSynonymGroup->toString().c_str());

	}

};

}
