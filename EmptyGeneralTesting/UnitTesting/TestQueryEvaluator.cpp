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
		ClauseSelectResultObject relObject;
		relObject.insertClauseSelectObject(selTrue);
		relObject.setBoolean(true);
        // 'Select s'
        ClauseSelectObject selFalse = ClauseSelectObject(CONSTANT, "s", AttrType::VAR_NAME, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selFalse);
		relObject1.setBoolean(false);
        // Relationships holds
        Assert::AreEqual(std::string("true"), evaluator->evaluateSelect(relObject, true)[0]);
        // Relationships don't holds
        Assert::AreEqual(std::string("false"), evaluator->evaluateSelect(relObject, false)[0]);
        int resultsSize = evaluator->evaluateSelect(relObject1, false).size();
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj , false);
        Assert::IsTrue(resultObj->getResultsBoolean());
		Logger::WriteMessage("=============");

		// FOLLOWS (1,4) <--- False
        ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(STMT, std::string(""), 1, false);
        ClauseSuchThatArgObject* arg4 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
        ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, arg3, arg4);
        ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1, false);
        Assert::IsFalse(re1->getResultsBoolean());

        // SELECT BOOLEAN such that Follows(4,5)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());

        // SELECT s such that Follows(4,5)
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "s"
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s,10)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s,10)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated statements
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(a,8)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Follows(a,8)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "s"
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(10,s)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(10,s)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "a"
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(16,a)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Follows(16,a)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(_,10);
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(_,10);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = {2, 3, 5, 6, 8, 9, 10, 13, 14, 15, 17}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(_,s);
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(_,s);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

        // SELECT BOOLEAN such that Follows(8,_);
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(8,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = { 1,2,4,5,6,7,8,10,13,14,16 }
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s,_);
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// FOLLOWS (s1, s2) ~
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s1 = {1,2,4,5,6,7,8,10,13,14,16}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s1");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

		// s2 = {2,3,5,6,8,9,10,13,14,15,17}
		std::vector<StmtNumber> s2Values = { 2,3,5,6,8,9,10,13,14,15,17 };
        std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("s2");
		int index = 0;
        for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
			Assert::AreEqual(s2Values[index], *it);
            index++;
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(updated1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s1,s2)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());

        Logger::WriteMessage("=============");

        // SELECT s1 such that Follows(s1,s2)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(results1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT s2 such that Follows(s1,s2)
        ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
        std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(11).c_str(), std::to_string(results2.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s1,s2)
        ClauseSelectObject selObject3 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
        std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// MODIFIES (3, "y") --> FALSE
        ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(STMT, std::string("y"), 0, false);
        ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, argOne, arg3);
        ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1, false);
        Assert::IsFalse(re1->getResultsBoolean());

        // SELECT BOOLEAN such that (Modifies(4,"x"))
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(4,"x"))
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "v"
        std::set<VarName> updated = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(9,v))
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that (Modifies(9,v))
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(13,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that (Modifies(13,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
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
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(s,"x")
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
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
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(s,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());
    }
	
	// MODIFIES (s,v) ~
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
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// s = {1,2,4,5,7,9,11,12,13,14,15,16,17}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(updated.size()).c_str());

		// v = { i, v, x, y, z }
		std::vector<VarName> vValues = { "i", "v", "x", "y", "z" };
		int index = 0;
        std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(vValues[index], *it);
			index++;
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Modifies(s,v);
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Modifies(s,v);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT v such that Modifies(s,v);
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
        std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results2.size()).c_str());
    }
	
	// PATTERN a(v,_) ~
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
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
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
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Pattern a(v,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(v,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that Pattern a(v,_)
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
        std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results2.size()).c_str());
    }

	// PATTERN a(_,_) 
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
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

        // SELECT BOOLEAN such that Pattern a(_,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(_,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
	
	// PATTERN a("x",_)
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
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = {1,4,7,11,15}
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that Pattern a("x",_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a("x",_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
	
	// PATTERN a(v,"_x_") ~
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
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
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
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(v,"_x_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that Pattern a(v,"_x_")
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
        std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());
    }

	// PATTERN a(_, "_x_")
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
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = { 7,11,13 }
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that a(_, "_x_")
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(_, "_x_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }

	// PATTERN a("z","_z_")
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
        ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
        Assert::IsTrue(resultObj->getResultsBoolean());

		// a = { 13 }
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that Pattern a("z","_z_")
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
        std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a("z","_z_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
        std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());
		Logger::WriteMessage("=============");

		// CALLS ("First", "Third") <--- False
		ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(PROCEDURE, std::string("First"), 0, false);
		ClauseSuchThatArgObject* arg4 = new ClauseSuchThatArgObject(PROCEDURE, std::string("Third"), 0, false);
		ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, arg3, arg4);
		ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1, false);
		Assert::IsFalse(re1->getResultsBoolean());

		// SELECT BOOLEAN such that CALLS ("First", "Second")
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());

		// SELECT s such that CALLS ("First", "Second")
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "p"
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (p,"Third")
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (p,"Third")
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "p"
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS ("First",p)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS ("First",p)
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (_,"Second");
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (_,"Second");
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p = { 1, 2 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (_,p);
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (_,p);
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// SELECT BOOLEAN such that CALLS ("First",_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS ("First",_)
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p = { 0, 1 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (p,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (p,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// CALLS (p1, p2) ~
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

		// Print procedure index
		std::set<ProcIndex> current = resultManager->getValuesForSynonym("p1");
		for (std::set<ProcIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(current.size()).c_str());
		Logger::WriteMessage("=============");

		std::set<ProcIndex> current1 = resultManager->getValuesForSynonym("p2");
		for (std::set<ProcIndex>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(current1.size()).c_str());
		Logger::WriteMessage("=============");

		// CALLS (p1, p2) 
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p1 = { 0, 1 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("p1");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());
		Logger::WriteMessage("=============");

		// p2 = { 1, 2 }
		std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("p2");
		std::vector<ProcIndex> p2Values = { 1, 2 };
		int index = 0;
		for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
			Assert::AreEqual(p2Values[index], *it);
			index++;
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that CALLS (p1, p2) 
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());

		Logger::WriteMessage("=============");

		// SELECT p1 such that CALLS (p1, p2) 
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT p2 such that CALLS (p1, p2) 
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "p2", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s such that CALLS (p1, p2) 
		ClauseSelectObject selObject3 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
		std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results3.size()).c_str());
	}
	
	// MODIFIES_P ("First", "x")
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// MODIFIES ("First", "y") --> TRUE
		ClauseSuchThatArgObject* arg3 = new ClauseSuchThatArgObject(STMT, std::string("y"), 0, false);
		ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, argOne, arg3);
		ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1, false);
		Assert::IsTrue(re1->getResultsBoolean());

		// SELECT BOOLEAN such that MODIFIES ("First", "x")
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that MODIFIES ("First", "x")
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());
	}

	// MODIFIES_P ("First",v)
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Get updated "v"
		std::set<VarName> updated = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that MODIFIES ("First",v)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that MODIFIES ("First",v)
		ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
	}
	
	// MODIFIES_P ("Second",_)
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		Logger::WriteMessage("=============");

		// SELECT BOOLEAN such that MODIFIES ("Second",_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that MODIFIES ("Second",_)
		ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
	}
	
	// MODIFIES_P (p,"x")
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
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
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p such that MODIFIES (p,"x")
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
	}
	
	// MODIFIES_P (p,_)
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
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
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p such that MODIFIES (p,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
	}

	// MODIFIES_P (p,v) ~
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
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
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
		std::vector<VarName> vValues = { "i", "v", "x", "y", "z" };
		int index = 0;
		for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(vValues[index], *it);
			index++;
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN such that MODIFIES (p,v);
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p such that MODIFIES (p,v);
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT v such that MODIFIES (p,v);
		ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// p.procName = "First"
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With1) {
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

		// Print procedures
		std::set<ProcIndex> current = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// p.procName = "First"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "First");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p = { 0 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with p.procName = "First"
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p with p.procName = "First"
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with p.procName = "First"
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}

	// p1.procName = p2.procName
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With2) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print procedures
		std::set<ProcIndex> current = resultManager->getValuesForSynonym("p1");
		for (std::set<ProcIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<ProcIndex> currents = resultManager->getValuesForSynonym("p2");
		for (std::set<ProcIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// p1.procName = p2.procName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p2", AttrType::PROC_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p1 = { 0, 1, 2 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p1");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

		// p2 = { 0, 1, 2 }
		std::set<ProcIndex> updated1 = resultManager->getValuesForSynonym("p2");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with p1.procName = p2.procName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p1 with p1.procName = p2.procName
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with p1.procName = p2.procName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}

	// p1.procName = c1.procName
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With3) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject c1(CALL, "c1");
		synonymTable->insert(c1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print procedures
		std::set<ProcIndex> current = resultManager->getValuesForSynonym("p1");
		for (std::set<ProcIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<StmtNumber> currents = resultManager->getValuesForSynonym("c1");
		for (std::set<ProcIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// p1.procName = c1.procName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CALL, "c1", AttrType::PROC_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p1 = { 1, 2 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p1");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		// c1 = { 3, 8 }
		std::set<ProcIndex> updated1 = resultManager->getValuesForSynonym("c1");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with p1.procName = c1.procName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p1 with p1.procName = c1.procName
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with p1.procName = p2.procName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}

	// p1.procName = v.varName
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With4) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(v);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print procedures
		std::set<ProcIndex> current = resultManager->getValuesForSynonym("p1");
		for (std::set<ProcIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<VarIndex> currents = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// p1.procName = v.varName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v", AttrType::VAR_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsFalse(resultObj->getResultsBoolean());

		// p1 = { 0, 1, 2 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("p1");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

		// v = { 0, 1, 2, 3, 4 }
		std::set<VarIndex> updated1 = resultManager->getValuesForSynonym("v");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN with p1.procName = v.varName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("false", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT p1 with p1.procName = v.varName
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(0).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with p1.procName = v.varName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(0).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// c.procName = "Second"
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With5) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c(CALL, "c");
		synonymTable->insert(c);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print calls
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("c");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// c.procName = "Second"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, CALL, "c", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// c = { 3 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("c");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with c.procName = "Second"
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT c with c.procName = "Second"
		ClauseSelectObject selObject1 = ClauseSelectObject(CALL, "c", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with c.procName = "Second"
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// c.procName = p.procName
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With6) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c(CALL, "c");
		synonymTable->insert(c);
		SynonymObject p(PROCEDURE, "p");
		synonymTable->insert(p);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print calls
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("c");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<ProcIndex> currents = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// c.procName = p.procName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, CALL, "c", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p", AttrType::PROC_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// c = { 3, 8 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("c");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		// p = { 0, 1, 2 }
		std::set<ProcIndex> updated1 = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN with c.procName = p.procName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT c with c.procName = p.procName
		ClauseSelectObject selObject1 = ClauseSelectObject(CALL, "c", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with c.procName = p.procName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// c1.procName = c2.procName
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With7) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c1(CALL, "c1");
		synonymTable->insert(c1);
		SynonymObject c2(CALL, "c2");
		synonymTable->insert(c2);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print calls
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("c1");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<StmtNumber> currents = resultManager->getValuesForSynonym("c2");
		for (std::set<ProcIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// c1.procName = c2.procName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, CALL, "c1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CALL, "c2", AttrType::PROC_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// c1 = { 3, 8 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("c1");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		// c2 = { 3, 8 }
		std::set<ProcIndex> updated1 = resultManager->getValuesForSynonym("c2");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN with c1.procName = c2.procName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT c1 with c1.procName = c2.procName
		ClauseSelectObject selObject1 = ClauseSelectObject(CALL, "c1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with c1.procName = c2.procName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}

	// c.procName = v.varName
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With8) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c(CALL, "c");
		synonymTable->insert(c);
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(v);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("c");
		for (std::set<ProcIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<VarIndex> currents = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// c.procName = v.varName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, CALL, "c", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v", AttrType::VAR_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsFalse(resultObj->getResultsBoolean());

		// c = { 3, 8 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("c");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		// v = { 0, 1, 2, 3, 4 }
		std::set<VarIndex> updated1 = resultManager->getValuesForSynonym("v");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN with c.procName = v.varName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("false", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT c with c.procName = v.varName
		ClauseSelectObject selObject1 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(0).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with c.procName = v.varName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(0).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// s.stmt# = 3
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With9) {
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
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// s.stmt# = 3
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(INTEGER, STMT, 3);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// s = { 3 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("s");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with s.stmt# = 3
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s with s.stmt# = 3
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// s.stmt# = c.value
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With10) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print constant
		std::set<Constant> current = resultManager->getValuesForSynonym("c");
		for (std::set<Constant>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// s.stmt# = c.value
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// s = { 1, 2, 3, 5 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("s");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(4).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with s.stmt# = c.value
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s with s.stmt# = c.value
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// s.stmt#  = a.stmt#
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With11) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject a(ASSIGN, "a");
		synonymTable->insert(a);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print assigns
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// s.stmt#  = a.stmt#
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, ASSIGN, "a", AttrType::STMT_NO);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// s = { 1, 2, 4, 5, 7, 9, 11, 12, 13, 14, 15, 16, 17 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("s");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(13).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with s.stmt#  = a.stmt#
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s with s.stmt#  = a.stmt#
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(13).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// v.varName = "x"
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With12) {
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

		// Print variables
		std::set<VarIndex> current = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// v.varName = "x"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v", AttrType::VAR_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, VARIABLE, "x");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// v = { 2 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("v");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with v.varName = "x"
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT v with v.varName = "x"
		ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with v.varName = "x"
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}

	// v.varName = p.procName;
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With13) {
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

		// Print variables
		std::set<VarIndex> current = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<ProcIndex> currents = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// v.varName = p.procName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v", AttrType::VAR_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p", AttrType::PROC_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsFalse(resultObj->getResultsBoolean());

		// v = { 0, 1, 2, 3, 4 }
		std::set<VarIndex> updated = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated.size()).c_str());

		// p = { 0, 1, 2 }
		std::set<ProcIndex> updated1 = resultManager->getValuesForSynonym("p");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN with v.varName = p.procName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("false", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT v with v.varName = p.procName
		ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(0).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with c.procName = p.procName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(0).c_str(), std::to_string(results2.size()).c_str());
	}

	// v1.varName = v2.varName;
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With14) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject v2(VARIABLE, "v2");
		synonymTable->insert(v2);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print variables
		std::set<VarIndex> current = resultManager->getValuesForSynonym("v1");
		for (std::set<VarIndex>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<VarIndex> currents = resultManager->getValuesForSynonym("v2");
		for (std::set<VarIndex>::iterator it = currents.begin(); it != currents.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// v1.varName = v2.varName
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v1", AttrType::VAR_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v2", AttrType::VAR_NAME);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// v1 = { 0, 1, 2, 3, 4 }
		std::set<VarIndex> updated = resultManager->getValuesForSynonym("v1");
		for (std::set<VarIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated.size()).c_str());

		// v2 = { 0, 1, 2, 3, 4 }
		std::set<VarIndex> updated1 = resultManager->getValuesForSynonym("v2");
		for (std::set<VarIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN with v1.varName = v2.varName
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT v1 with v1.varName = v2.varName
		ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with v1.varName = v2.varName
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}

	// c.value = 3
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With15) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print constant
		std::set<Constant> current = resultManager->getValuesForSynonym("c");
		for (std::set<Constant>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// c.value = 3
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithRefObject rightObj = ClauseWithRefObject(INTEGER, CONSTANT, 3);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// c = { 3 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("c");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with c.value = 3
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s with c.value = 3
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}
	
	// c1.value = c2.value
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With16) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c1(CONSTANT, "c1");
		synonymTable->insert(c1);
		SynonymObject c2(CONSTANT, "c2");
		synonymTable->insert(c2);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print constant
		std::set<Constant> current = resultManager->getValuesForSynonym("c1");
		for (std::set<Constant>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print constant
		std::set<Constant> current2 = resultManager->getValuesForSynonym("c2");
		for (std::set<Constant>::iterator it = current2.begin(); it != current2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// c1.value = c2.value
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c1", AttrType::VALUE);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c2", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// c1 = { 0, 1, 2, 3, 5 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("c1");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with c1.value = c2.value
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s with c1.value = c2.value
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}
	
	// c.value = s.stmt#
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With17) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print constant
		std::set<Constant> current = resultManager->getValuesForSynonym("c");
		for (std::set<Constant>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// c.value = s.stmt#
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, STMT, "s", AttrType::STMT_NO);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// c = { 1, 2, 3, 5 }
		std::set<ProcIndex> updated = resultManager->getValuesForSynonym("c");
		for (std::set<ProcIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(4).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with c.value = s.stmt#
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s with c.value = s.stmt#
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// n = 10
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With18) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject n(PROGRAM_LINE, "n");
		synonymTable->insert(n);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print program lines
		std::set<ProgLineNumber> current = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// n = 10
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(SYNONYM, PROGRAM_LINE, "n");
		ClauseWithRefObject rightObj = ClauseWithRefObject(INTEGER, CONSTANT, 10);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// n = { 10 }
		std::set<ProgLineNumber> updated = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with n = 10
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s with n = 10
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// n = n1
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With19) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject n(PROGRAM_LINE, "n");
		synonymTable->insert(n);
		SynonymObject n1(PROGRAM_LINE, "n1");
		synonymTable->insert(n1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print program lines
		std::set<ProgLineNumber> current = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<ProgLineNumber> current2 = resultManager->getValuesForSynonym("n1");
		for (std::set<ProgLineNumber>::iterator it = current2.begin(); it != current2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// n = n1
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(SYNONYM, PROGRAM_LINE, "n");
		ClauseWithRefObject rightObj = ClauseWithRefObject(SYNONYM, PROGRAM_LINE, "n1");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// n = { 1-17 }
		std::set<ProgLineNumber> updated = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with n = n1
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT n with n = n1
		ClauseSelectObject selObject2 = ClauseSelectObject(PROGRAM_LINE, "n", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with n = n1
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// n = s.stmt#
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With20) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject n(PROGRAM_LINE, "n");
		synonymTable->insert(n);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print program lines
		std::set<ProgLineNumber> current = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// n = s.stmt#
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(SYNONYM, PROGRAM_LINE, "n");
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, PROGRAM_LINE, "s", AttrType::STMT_NO);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// n = { 1-17 }
		std::set<ProgLineNumber> updated = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with n = s.stmt#
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT n with n = s.stmt#
		ClauseSelectObject selObject2 = ClauseSelectObject(PROGRAM_LINE, "n", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with n = s.stmt#
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// n = c.value
	TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_With21) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		SynonymObject n(PROGRAM_LINE, "n");
		synonymTable->insert(n);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print program lines
		std::set<ProgLineNumber> current = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		std::set<ProgLineNumber> current2 = resultManager->getValuesForSynonym("c");
		for (std::set<ProgLineNumber>::iterator it = current2.begin(); it != current2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// n = c.value
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(SYNONYM, PROGRAM_LINE, "n");
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// n = { 1, 2, 3, 5 }
		std::set<ProgLineNumber> updated = resultManager->getValuesForSynonym("n");
		for (std::set<ProgLineNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(4).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN with n = c.value
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT n with n = c.value
		ClauseSelectObject selObject2 = ClauseSelectObject(PROGRAM_LINE, "n", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results2.size()).c_str());

		Logger::WriteMessage("=============");

		// SELECT s with n = c.value
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results1.size()).c_str());

		Logger::WriteMessage("=============");

	}

	// PATTERN W(V,_) ~
	TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern_While1) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject w(WHILE, "w");
		synonymTable->insert(w);
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(v);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print while-statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("w");
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

		// PATTERN w(v,_)
		EntityType patternType = WHILE;
		EntityType firstArgType = VARIABLE;
		std::string patternSynonymArg = "w";
		std::string firstArg = "v";
		std::string secondArg = "_";
		bool isFirstArgSynonym = true;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
		ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// w = { 6 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("w");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// v = { i }
		std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual("i", (*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN such that PATTERN w(v,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT w such that PATTERN w(v,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(WHILE, "w", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that PATTERN w(v,_)
		ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// PATTERN W(_,_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern_While2) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject w(WHILE, "w");
		synonymTable->insert(w);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print while-statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("w");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// PATTERN W(_,_)
		EntityType patternType = WHILE;
		EntityType firstArgType = WILDCARD;
		std::string patternSynonymArg = "w";
		std::string firstArg = "_";
		std::string secondArg = "_";
		bool isFirstArgSynonym = false;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
		ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// w = { 6 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("w");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN such that PATTERN W(_,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT w such that PATTERN W(_,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(WHILE, "w", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that PATTERN W(_,_)
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// PATTERN W("i",_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern_While3) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject w(WHILE, "w");
		synonymTable->insert(w);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print while-statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("w");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// PATTERN W("i",_)
		EntityType patternType = WHILE;
		EntityType firstArgType = VARIABLE;
		std::string patternSynonymArg = "w";
		std::string firstArg = "i";
		std::string secondArg = "_";
		bool isFirstArgSynonym = false;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
		ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// w = { 6 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("w");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN such that PATTERN W("i",_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT w such that PATTERN W("i",_)
		ClauseSelectObject selObject1 = ClauseSelectObject(WHILE, "w", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that PATTERN W("i",_)
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}

	// PATTERN IF(V,_,_) ~
	TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern_If1) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject ifstat(IF, "ifstat");
		synonymTable->insert(ifstat);
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(v);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print if-statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("ifstat");
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

		// PATTERN IF(V,_,_)
		EntityType patternType = IF;
		EntityType firstArgType = VARIABLE;
		std::string patternSynonymArg = "ifstat";
		std::string firstArg = "v";
		std::string secondArg = "_";
		std::string thirdArg = "_";
		bool isFirstArgSynonym = true;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg, thirdArg);
		ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// ifstat = { 10 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("ifstat");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// v = { x }
		std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual("x", (*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated1.size()).c_str());

		// SELECT BOOLEAN such that PATTERN IF(V,_,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT ifstat such that PATTERN IF(V,_,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(IF, "ifstat", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that PATTERN IF(V,_,_)
		ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
	}

	// PATTERN IF(_,_,_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern_If2) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject ifstat(IF, "ifstat");
		synonymTable->insert(ifstat);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print if-statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("ifstat");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// PATTERN IF(_,_,_)
		EntityType patternType = IF;
		EntityType firstArgType = WILDCARD;
		std::string patternSynonymArg = "ifstat";
		std::string firstArg = "_";
		std::string secondArg = "_";
		std::string thirdArg = "_";
		bool isFirstArgSynonym = false;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg, thirdArg);
		ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// ifstat = { 10 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("ifstat");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN such that PATTERN IF(_,_,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT ifstat such that PATTERN IF(_,_,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(IF, "ifstat", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that PATTERN IF(V,_,_)
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// PATTERN IF("x",_,_)
	TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern_If3) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject ifstat(IF, "ifstat");
		synonymTable->insert(ifstat);
		SynonymObject s(STMT, "s");
		synonymTable->insert(s);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print if-statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("ifstat");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print statements
		std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// PATTERN IF("x",_,_)
		EntityType patternType = IF;
		EntityType firstArgType = VARIABLE;
		std::string patternSynonymArg = "ifstat";
		std::string firstArg = "x";
		std::string secondArg = "_";
		std::string thirdArg = "_";
		bool isFirstArgSynonym = false;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg, thirdArg);
		ClausePatternObject* resultObj = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// ifstat = { 10 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("ifstat");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// SELECT BOOLEAN such that PATTERN IF("x",_,_)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT ifstat such that PATTERN IF("x",_,_)
		ClauseSelectObject selObject1 = ClauseSelectObject(IF, "ifstat", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that PATTERN IF("x",_,_)
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(17).c_str(), std::to_string(results2.size()).c_str());
	}
	
	// PATTERN a(v,_) and FOLLOWS (a,8) ~
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi1) {
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

		// Print assignment statements
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

		// Pattern a(v,_)
		EntityType patternType = ASSIGN;
		EntityType firstArgType = VARIABLE;
		std::string patternSynonymArg = "a";
		std::string firstArg = "v";
		std::string secondArg = "_";
		bool isFirstArgSynonym = true;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
		ClausePatternObject* resultObj1 = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// a = { 1, 2, 4, 5, 7, 9, 11, 12, 13, 14, 15, 16, 17 }
		std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("a");
		for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
		Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(13).c_str(), std::to_string(updated1.size()).c_str());

		// v = { 0, 1, 2, 3, 4 }
		std::set<VarIndex> updated2 = resultManager->getValuesForSynonym("v");
		for (std::set<StmtNumber>::iterator it = updated2.begin(); it != updated2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated2.size()).c_str());

		// FOLLOWS (a,8)
		RelationshipType type = FOLLOWS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(ASSIGN, std::string("a"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string(""), 8, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// a = { 7 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// v = { 2 }
		std::set<VarIndex> updated3 = resultManager->getValuesForSynonym("v");
		for (std::set<StmtNumber>::iterator it = updated3.begin(); it != updated3.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated3.size()).c_str());

		// SELECT BOOLEAN such that PATTERN a(v,_) and FOLLOWS (a,8)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean() && resultObj1->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
		Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT a such that PATTERN a(v,_) and FOLLOWS (a,8)
		ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
		Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that PATTERN a(v,_) and FOLLOWS (a,8)
		ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");
	}

	// MODIFIES (9, v) and PATTERN a(v, _)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi2) {
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

		// Print assignment-statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
		Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Print variable names
		std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
		Logger::WriteMessage((*it).c_str());
		}
		Logger::WriteMessage("=============");

		// Modifies(9,v)
		RelationshipType type = MODIFIES;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string(""), 9, false);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// v = { 0 } 
		std::set<VarIndex> updated = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
		Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

		// Pattern a(v,_)
		EntityType patternType = ASSIGN;
		EntityType firstArgType = VARIABLE;
		std::string patternSynonymArg = "a";
		std::string firstArg = "v";
		std::string secondArg = "_";
		bool isFirstArgSynonym = true;
		ClausePatternObject* patternObj = new ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
		ClausePatternObject* resultObj1 = evaluator->evaluatePattern(patternObj, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// a = { 5, 9 }
		std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("a");
		for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
		Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

		// v = { 0 }
		std::set<VarIndex> updated2 = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = updated2.begin(); it != updated2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated2.size()).c_str());

		// SELECT BOOLEAN such that MODIFIES (9, v) and PATTERN a(v, _)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean() && resultObj1->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
		Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT a such that MODIFIES (9, v) and PATTERN a(v, _)
		ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
		Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");
	}
	
	// PARENT (s1,s2) and FOLLOWS (s2,s3)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi3) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject s3(STMT, "s3");
		synonymTable->insert(s3);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Print statements
		std::set<StmtNumber> current = resultManager->getValuesForSynonym("s1");
		for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");

		// PARENT (s1,s2)
		RelationshipType type = PARENT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// s1 = { 6, 10 }
		std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("s1");
		for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

		// s2 = { 7, 8, 9, 11, 12 }
		std::set<StmtNumber> updated2 = resultManager->getValuesForSynonym("s2");
		for (std::set<StmtNumber>::iterator it = updated2.begin(); it != updated2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated2.size()).c_str());

		// FOLLOWS (s2,s3)
		RelationshipType type1 = FOLLOWS;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new ClauseSuchThatArgObject(STMT, std::string("s3"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);
		ClauseSuchThatObject* resultObj1 = evaluator->evaluateSuchThat(suchThatObj1, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// s2 = { 7, 8 }
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s2");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		// s3 = { 8, 9 }
		std::set<StmtNumber> updated3 = resultManager->getValuesForSynonym("s3");
		for (std::set<StmtNumber>::iterator it = updated3.begin(); it != updated3.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated3.size()).c_str());

		// SELECT BOOLEAN such that PARENT (s1,s2) and FOLLOWS (s2,s3)
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean() && resultObj1->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s1 such that PARENT (s1,s2) and FOLLOWS (s2,s3)
		// s1 = { 6 }
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT s2 such that PARENT (s1,s2) and FOLLOWS (s2,s3)
		// s2 = { 7, 8 }
		ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT s3 such that PARENT (s1,s2) and FOLLOWS (s2,s3)
		// s3 = { 8, 9 }
		ClauseSelectObject selObject3 = ClauseSelectObject(STMT, "s3", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
		std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results3.size()).c_str());
		Logger::WriteMessage("=============");
	}
	
	// MODIFIES (s, v) with v.varName = "x"
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi4) {
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

		// Print variable names
		std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
		for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Logger::WriteMessage("=============");

		// MODIFIES (s, v)
		RelationshipType type = MODIFIES;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// s = { 1,2,4,5,7,9,11,12,13,14,15,16,17 } 
		std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(13).c_str(), std::to_string(updated.size()).c_str());

		// v = { 0, 1, 2, 3, 4 } 
		std::set<VarIndex> updated1 = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated1.size()).c_str());

		// v.varName = "x"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v", AttrType::VAR_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, VARIABLE, "x");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj1 = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// v = { 2 }
		std::set<VarIndex> updated2 = resultManager->getValuesForSynonym("v");
		for (std::set<VarIndex>::iterator it = updated2.begin(); it != updated2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated2.size()).c_str());

		// s = { 1, 4, 7, 11, 15 }
		std::set<StmtNumber> updated3 = resultManager->getValuesForSynonym("s");
		for (std::set<StmtNumber>::iterator it = updated3.begin(); it != updated3.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(updated3.size()).c_str());

		// SELECT BOOLEAN such that MODIFIES (s, v) with v.varName = "x"
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean() && resultObj1->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s such that MODIFIES (s, v) with v.varName = "x"
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v such that MODIFIES (s, v) with v.varName = "x"
		ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");
	}
	
	// Modifies_P (p1, v1) and Uses_P (p1, v1) with p1.procName = "Third"
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi5) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// MODIFIES_P (p1, v1)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Uses_P (p1, v1)
		RelationshipType type1 = USES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);
		ClauseSuchThatObject* resultObj1 = evaluator->evaluateSuchThat(suchThatObj1, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// p1.procName = "Third"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Third");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj2 = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj2->getResultsBoolean());

		// SELECT p1 such that Modifies_P (p1, v1) and Uses_P (p1, v1) with p1.procName = "Third"
		// p1 = { 2 }
		ClauseSelectObject selObject2 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v1 such that Modifies_P (p1, v1) and Uses_P (p1, v1) with p1.procName = "Third"
		// v1 = { z }
		ClauseSelectObject selObject3 = ClauseSelectObject(VARIABLE, "v1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
		std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results3.size()).c_str());
		Logger::WriteMessage("=============");

	}

	// Modifies_P (p1, v1) and Uses_P (p1, v1) with p1.procName = "Second"
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi6) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// MODIFIES_P (p1, v1)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Uses_P (p1, v1)
		RelationshipType type1 = USES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);
		ClauseSuchThatObject* resultObj1 = evaluator->evaluateSuchThat(suchThatObj1, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// p1.procName = "Second"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj2 = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj2->getResultsBoolean());

		// SELECT p1 such that Modifies_P (p1, v1) and Uses_P (p1, v1) with p1.procName = "Third"
		// p1 = { 1 }
		ClauseSelectObject selObject2 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v1 such that Modifies_P (p1, v1) and Uses_P (p1, v1) with p1.procName = "Third"
		// v1 = { i, x, y, z }
		ClauseSelectObject selObject3 = ClauseSelectObject(VARIABLE, "v1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
		std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results3.size()).c_str());
		Logger::WriteMessage("=============");

	}
	
	// Modifies_P (p1, v1) and Uses_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi7) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// MODIFIES_P (p1, v1)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Uses_P (p1, v1)
		RelationshipType type1 = USES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);
		ClauseSuchThatObject* resultObj1 = evaluator->evaluateSuchThat(suchThatObj1, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// Calls (p1, p2)
		RelationshipType type2 = CALLS;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);
		ClauseSuchThatObject* resultObj3 = evaluator->evaluateSuchThat(suchThatObj2, false);
		Assert::IsTrue(resultObj3->getResultsBoolean());

		// p1.procName = "Second"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj2 = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj2->getResultsBoolean());

		// SELECT p1 such that Modifies_P (p1, v1) and Uses_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
		// p1 = { 1 }
		ClauseSelectObject selObject2 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v1 such that Modifies_P (p1, v1) and Uses_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
		// v1 = { i, x, y, z }
		ClauseSelectObject selObject3 = ClauseSelectObject(VARIABLE, "v1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
		std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results3.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT p2 such that Modifies_P (p1, v1) and Uses_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
		// p2 = { 2 }
		ClauseSelectObject selObject4 = ClauseSelectObject(PROCEDURE, "p2", AttrType::INVALID, false);
		ClauseSelectResultObject relObject4;
		relObject4.insertClauseSelectObject(selObject4);
		std::vector<std::string> results4 = evaluator->evaluateSelect(relObject4, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results4.begin(); it != results4.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results4.size()).c_str());
		Logger::WriteMessage("=============");

	}

	// Modifies_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi8) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// MODIFIES_P (p1, v1)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Calls (p1, p2)
		RelationshipType type2 = CALLS;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);
		ClauseSuchThatObject* resultObj3 = evaluator->evaluateSuchThat(suchThatObj2, false);
		Assert::IsTrue(resultObj3->getResultsBoolean());

		// p1.procName = "Second"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj2 = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj2->getResultsBoolean());

		// SELECT p1 such that Modifies_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
		// p1 = { 1 }
		ClauseSelectObject selObject2 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v1 such that Modifies_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
		// v1 = { i, v, x, y, z }
		ClauseSelectObject selObject3 = ClauseSelectObject(VARIABLE, "v1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
		std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results3.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT p2 such that Modifies_P (p1, v1) and Calls(p1, p2) with p1.procName = "Second"
		// p2 = { 2 }
		ClauseSelectObject selObject4 = ClauseSelectObject(PROCEDURE, "p2", AttrType::INVALID, false);
		ClauseSelectResultObject relObject4;
		relObject4.insertClauseSelectObject(selObject4);
		std::vector<std::string> results4 = evaluator->evaluateSelect(relObject4, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results4.begin(); it != results4.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results4.size()).c_str());
		Logger::WriteMessage("=============");

	}

	// Modifies_P (p1, v1) and Calls(p1, p2) and Modifies_P (p2, v1) with p1.procName = "Second" ~
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi9) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// MODIFIES_P (p1, v1)
		RelationshipType type = MODIFIES_P;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Calls (p1, p2)
		RelationshipType type2 = CALLS;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);
		ClauseSuchThatObject* resultObj3 = evaluator->evaluateSuchThat(suchThatObj2, false);
		Assert::IsTrue(resultObj3->getResultsBoolean());

		// Modifies_P (p2, v1)
		RelationshipType type3 = MODIFIES_P;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);
		ClauseSuchThatObject* resultObj4 = evaluator->evaluateSuchThat(suchThatObj3, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// p1.procName = "Second"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj2 = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj2->getResultsBoolean());

		// p2 = { 2 } 
		std::set<ProcIndex> updated1 = resultManager->getValuesForSynonym("p2");
		for (std::set<ProcIndex>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated1.size()).c_str());

		// v1 = { 1, 4 } 
		std::set<VarIndex> updated = resultManager->getValuesForSynonym("v1");
		for (std::set<VarIndex>::iterator it = updated.begin(); it != updated.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

		// SELECT p1 such that Modifies_P (p1, v1) and Calls(p1, p2) and Modifies_P (p2, v1) with p1.procName = "Second"
		// p1 = { 1 }
		ClauseSelectObject selObject2 = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT p2 such that Modifies_P (p1, v1) and Calls(p1, p2) and Modifies_P (p2, v1) with p1.procName = "Second"
		// p2 = { 2 }
		ClauseSelectObject selObject4 = ClauseSelectObject(PROCEDURE, "p2", AttrType::INVALID, false);
		ClauseSelectResultObject relObject4;
		relObject4.insertClauseSelectObject(selObject4);
		std::vector<std::string> results4 = evaluator->evaluateSelect(relObject4, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results4.begin(); it != results4.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results4.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v1 such that Modifies_P (p1, v1) and Calls(p1, p2) and Modifies_P (p2, v1) with p1.procName = "Second"
		// v1 = { v, z }
		ClauseSelectObject selObject3 = ClauseSelectObject(VARIABLE, "v1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject3;
		relObject3.insertClauseSelectObject(selObject3);
		std::vector<std::string> results3 = evaluator->evaluateSelect(relObject3, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results3.size()).c_str());
		Logger::WriteMessage("=============");

	}

	// Modifies (s1,v1) and Uses (s1,v1) and v1.varName = "x" ~
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Multi10) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		ResultGridManager* resultManager = evaluator->populateResultGrids();

		// Modifies (s1,v1) 
		RelationshipType type = MODIFIES;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
		ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj, false);
		Assert::IsTrue(resultObj->getResultsBoolean());

		// Uses(s1, v1)
		RelationshipType type1 = USES;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);
		ClauseSuchThatObject* resultObj2 = evaluator->evaluateSuchThat(suchThatObj1, false);
		Assert::IsTrue(resultObj2->getResultsBoolean());

		// v1.varName = "x"
		// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, VARIABLE, "v1", AttrType::VAR_NAME);
		ClauseWithRefObject rightObj = ClauseWithRefObject(IDENTIFIER, VARIABLE, "x");
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);
		ClauseWithObject* resultObj1 = evaluator->evaluateWith(withThatObj, false);
		Assert::IsTrue(resultObj1->getResultsBoolean());

		// v1 = { 2 }
		std::set<VarIndex> updated2 = resultManager->getValuesForSynonym("v1");
		for (std::set<VarIndex>::iterator it = updated2.begin(); it != updated2.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated2.size()).c_str());

		// s1 = { 7, 11, 15 }
		std::set<StmtNumber> updated3 = resultManager->getValuesForSynonym("s1");
		for (std::set<StmtNumber>::iterator it = updated3.begin(); it != updated3.end(); ++it) {
			Logger::WriteMessage(std::to_string(*it).c_str());
		}
		Logger::WriteMessage("=============");
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated3.size()).c_str());

		// SELECT BOOLEAN such that Modifies (s1,v1) and Uses (s1,v1) and v1.varName = "x"
		ClauseSelectResultObject relObject;
		relObject.setBoolean(true);
		std::vector<std::string> results = evaluator->evaluateSelect(relObject, resultObj->getResultsBoolean() && resultObj1->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual("true", results.begin()->c_str());
		Logger::WriteMessage("=============");

		// SELECT s1 such that Modifies (s1,v1) and Uses (s1,v1) and v1.varName = "x"
		ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject1;
		relObject1.insertClauseSelectObject(selObject1);
		std::vector<std::string> results1 = evaluator->evaluateSelect(relObject1, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
		Logger::WriteMessage("=============");

		// SELECT v1 such that Modifies (s1,v1) and Uses (s1,v1) and v1.varName = "x"
		ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v1", AttrType::INVALID, false);
		ClauseSelectResultObject relObject2;
		relObject2.insertClauseSelectObject(selObject2);
		std::vector<std::string> results2 = evaluator->evaluateSelect(relObject2, resultObj->getResultsBoolean());
		for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
		}
		Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
		Logger::WriteMessage("=============");
	}
	
	// Select s1 such that Modifies(s2,"x") and Modifies(5,"i") and Modifies(s1,v1)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize1) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("v1", 2);
		
		// Select s1
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);


		// Modifies(s2,"x") 
		RelationshipType type = MODIFIES;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Modifies(5,"i") 
		RelationshipType type1 = MODIFIES;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("i"), 0, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,v1) 
		RelationshipType type2 = MODIFIES;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj2);

		std::vector<std::string> s1Values = { "1", "2", "4", "5", "7", "9", "11", "12", "13", "14", "15", "16", "17" };

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(s1Values[index], *it);
			index++;
		}

	}
	
	// Select s1 such that Modifies(s2,"x") and Modifies(5,"y") and Modifies(s1,v1)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize2) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("v1", 2);

		// Select s1
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);

		// Modifies(s2,"x") 
		RelationshipType type = MODIFIES;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Modifies(5,"i") 
		RelationshipType type1 = MODIFIES;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("y"), 0, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,v1) 
		RelationshipType type2 = MODIFIES;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj2);


		std::vector<std::string> results = evaluator->evaluate(queryTable);
		Assert::IsTrue(0==results.size());
	}

	// Select BOOLEAN such that Modifies(s2,"x") and Modifies(5,"y") and Modifies(s1,v1)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize3) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("v1", 2);

		// Select BOOLEAN
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s1", AttrType::INVALID, true);

		// Modifies(s2,"x") 
		RelationshipType type = MODIFIES;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Modifies(5,"i") 
		RelationshipType type1 = MODIFIES;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("y"), 0, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,v1) 
		RelationshipType type2 = MODIFIES;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj2);


		std::vector<std::string> results = evaluator->evaluate(queryTable);
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual("false", (*it).c_str());
		}
		
	}

	// Select s1 such that Parent(s1,s2) and Modifies(5,"i") and Uses(s1,v1) and Modifies(s3,v2) and Uses(s3,"x")
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize4) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject s3(STMT, "s3");
		synonymTable->insert(s3);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject v2(VARIABLE, "v2");
		synonymTable->insert(v2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("s3", 2);
		mSynonymGroup->insertSynonym("v2", 2);

		// Select s1
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);

		// Parent(s1,s2) 
		RelationshipType type = PARENT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Modifies(5,"i") 
		RelationshipType type1 = MODIFIES;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("i"), 0, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Uses(s1,v1)
		RelationshipType type2 = USES;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);

		// Modifies(s3,v2) 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s3"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v2"), 0, true);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// Uses(s3,"x")
		RelationshipType type4 = USES;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s3"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj2);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> s1Values = { "6" , "10" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(s1Values[index], (*it));
			index++;
		}
	}

	// Select s2 such that Parent(s1,s2) and Modifies(5,"i") and Uses(s1,v1) and Modifies(s3,v2) and Uses(s3,"x")
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize5) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject s3(STMT, "s3");
		synonymTable->insert(s3);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject v2(VARIABLE, "v2");
		synonymTable->insert(v2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("s3", 2);
		mSynonymGroup->insertSynonym("v2", 2);

		// Select s2
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);

		// Parent(s1,s2) 
		RelationshipType type = PARENT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Modifies(5,"i") 
		RelationshipType type1 = MODIFIES;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("i"), 0, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Uses(s1,v1)
		RelationshipType type2 = USES;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);

		// Modifies(s3,v2) 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s3"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v2"), 0, true);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// Uses(s3,"x")
		RelationshipType type4 = USES;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s3"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj2);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> s2Values = { "7", "8", "9", "11", "12" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(s2Values[index], (*it));
			index++;
		}
	}

	// Select BOOLEAN such that Parent(s1,s2) and Modifies(5,"i") and Uses(s1,v1) and Modifies(s3,v2) and Uses(s3,"x")
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize6) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject s3(STMT, "s3");
		synonymTable->insert(s3);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject v2(VARIABLE, "v2");
		synonymTable->insert(v2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("s3", 2);
		mSynonymGroup->insertSynonym("v2", 2);

		// Select BOOLEAN
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "", AttrType::INVALID, true);

		// Parent(s1,s2) 
		RelationshipType type = PARENT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Modifies(5,"i") 
		RelationshipType type1 = MODIFIES;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("i"), 0, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Uses(s1,v1)
		RelationshipType type2 = USES;
		ClauseSuchThatArgObject* argOne2 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo2 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj2 = new ClauseSuchThatObject(type2, argOne2, argTwo2);

		// Modifies(s3,v2) 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s3"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v2"), 0, true);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// Uses(s3,"x")
		RelationshipType type4 = USES;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s3"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj2);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::IsTrue("true"==(*it));
		}
	}
	
	// Select s1 such that Next(n1,n2) and Follows(4,5) and Modifies(s1,"x") with s1.stmt = c.value and Follows(s1,s2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize7) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject n1(PROGRAM_LINE, "n1");
		synonymTable->insert(n1);
		SynonymObject n2(PROGRAM_LINE, "n2");
		synonymTable->insert(n2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("c", 1);
		mSynonymGroup->insertSynonym("n1", 2);
		mSynonymGroup->insertSynonym("n2", 2);

		// Select s1
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);

		// Next(n1,n2) 
		RelationshipType type = NEXT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Follows(4,5)
		RelationshipType type1 = FOLLOWS;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,"x") 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);
	
		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> s1Values = { "1" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(s1Values[index], (*it));
			index++;
		}
	}

	// Select s2 such that Next(n1,n2) and Follows(4,5) and Modifies(s1,"x") with s1.stmt = c.value and Follows(s1,s2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize8) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject n1(PROGRAM_LINE, "n1");
		synonymTable->insert(n1);
		SynonymObject n2(PROGRAM_LINE, "n2");
		synonymTable->insert(n2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("c", 1);
		mSynonymGroup->insertSynonym("n1", 2);
		mSynonymGroup->insertSynonym("n2", 2);

		// Select s2
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);

		// Next(n1,n2) 
		RelationshipType type = NEXT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Follows(4,5)
		RelationshipType type1 = FOLLOWS;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,"x") 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> s2Values = { "2" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(s2Values[index], (*it));
			index++;
		}
	}

	// Select BOOLEAN such that Next(n1,n2) and Follows(4,5) and Modifies(s1,"x") with s1.stmt = c.value and Follows(s1,s2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize9) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject n1(PROGRAM_LINE, "n1");
		synonymTable->insert(n1);
		SynonymObject n2(PROGRAM_LINE, "n2");
		synonymTable->insert(n2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("c", 1);
		mSynonymGroup->insertSynonym("n1", 2);
		mSynonymGroup->insertSynonym("n2", 2);

		// Select BOOLEAN
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s2", AttrType::INVALID, true);

		// Next(n1,n2) 
		RelationshipType type = NEXT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Follows(4,5)
		RelationshipType type1 = FOLLOWS;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,"x") 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::IsTrue("true"==(*it));
		}
	}

	// Select BOOLEAN such that Next(n1,n2) and Follows(4,7) <- FALSE and Modifies(s1,"x") with s1.stmt = c.value and Follows(s1,s2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize10) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject n1(PROGRAM_LINE, "n1");
		synonymTable->insert(n1);
		SynonymObject n2(PROGRAM_LINE, "n2");
		synonymTable->insert(n2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("c", 1);
		mSynonymGroup->insertSynonym("n1", 2);
		mSynonymGroup->insertSynonym("n2", 2);

		// Select BOOLEAN
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s2", AttrType::INVALID, true);

		// Next(n1,n2) 
		RelationshipType type = NEXT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Follows(4,7)
		RelationshipType type1 = FOLLOWS;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(STMT, std::string(""), 7, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,"x") 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::IsTrue("false" == (*it));
		}
	}

	// Select p1 such that Modifies_P (p1,v1) with p1.procName = "Second" and Follows(s1,s2) and s1.stmt# = c.value and Calls(p1,p2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize11) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("p1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("p2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("s2", 2);
		mSynonymGroup->insertSynonym("c", 2);

		// Select p1
		ClauseSelectObject selObject = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);

		// Modifies_P (p1,v1)
		RelationshipType type1 = MODIFIES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// with p1.procName = "Second"
		ClauseWithRefObject leftObj1 = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj1 = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj1 = new ClauseWithObject(&leftObj1, &rightObj1);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Calls(p1,p2)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj4);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertWithObject(withThatObj1);
	
		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> p1Values = { "Second" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(p1Values[index], (*it));
			index++;
		}
	}
	
	// Select p2 such that Modifies_P (p1,v1) with p1.procName = "Second" and Follows(s1,s2) and s1.stmt# = c.value and Calls(p1,p2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize12) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("p1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("p2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("s2", 2);
		mSynonymGroup->insertSynonym("c", 2);

		// Select p2
		ClauseSelectObject selObject = ClauseSelectObject(PROCEDURE, "p2", AttrType::INVALID, false);

		// Modifies_P (p1,v1)
		RelationshipType type1 = MODIFIES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// with p1.procName = "Second"
		ClauseWithRefObject leftObj1 = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj1 = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj1 = new ClauseWithObject(&leftObj1, &rightObj1);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Calls(p1,p2)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj4);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertWithObject(withThatObj1);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> p2Values = { "Third" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(p2Values[index], (*it));
			index++;
		}
	}
	
	// Select BOOLEAN such that Modifies_P (p1,v1) with p1.procName = "Second" and Follows(s1,s2) and s1.stmt# = c.value and Calls(p1,p2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize13) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("p1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("p2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("s2", 2);
		mSynonymGroup->insertSynonym("c", 2);

		// Select BOOLEAN
		ClauseSelectObject selObject = ClauseSelectObject(PROCEDURE, "p2", AttrType::INVALID, true);

		// Modifies_P (p1,v1)
		RelationshipType type1 = MODIFIES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// with p1.procName = "Second"
		ClauseWithRefObject leftObj1 = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj1 = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj1 = new ClauseWithObject(&leftObj1, &rightObj1);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Calls(p1,p2)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj4);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertWithObject(withThatObj1);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> boolean = { "true" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(boolean[index], (*it));
			index++;
		}
	}

	// Select s1 such that Modifies_P (p1,v1) with p1.procName = "Second" and Follows(s1,s2) and s1.stmt# = c.value and Calls(p1,p2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize14) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("p1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("p2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("s2", 2);
		mSynonymGroup->insertSynonym("c", 2);

		// Select s1
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);

		// Modifies_P (p1,v1)
		RelationshipType type1 = MODIFIES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// with p1.procName = "Second"
		ClauseWithRefObject leftObj1 = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj1 = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj1 = new ClauseWithObject(&leftObj1, &rightObj1);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Calls(p1,p2)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj4);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertWithObject(withThatObj1);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> s1Values = { "1", "2", "5" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(s1Values[index], (*it));
			index++;
		}
	}

	// Select s2 such that Modifies_P (p1,v1) with p1.procName = "Second" and Follows(s1,s2) and s1.stmt# = c.value and Calls(p1,p2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize15) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("p1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("p2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("s2", 2);
		mSynonymGroup->insertSynonym("c", 2);

		// Select s2
		ClauseSelectObject selObject = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);

		// Modifies_P (p1,v1)
		RelationshipType type1 = MODIFIES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// with p1.procName = "Second"
		ClauseWithRefObject leftObj1 = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj1 = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Second");
		ClauseWithObject* withThatObj1 = new ClauseWithObject(&leftObj1, &rightObj1);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Calls(p1,p2)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj4);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertWithObject(withThatObj1);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> s2Values = { "2", "3", "6" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(s2Values[index], (*it));
			index++;
		}
	}

	// Select p1 such that Modifies_P (p1,v1) with p1.procName = "Fourth" <-- FALSE and Follows(s1,s2) and s1.stmt# = c.value and Calls(p1,p2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize16) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject v1(VARIABLE, "v1");
		synonymTable->insert(v1);
		SynonymObject p1(PROCEDURE, "p1");
		synonymTable->insert(p1);
		SynonymObject p2(PROCEDURE, "p2");
		synonymTable->insert(p2);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("p1", 1);
		mSynonymGroup->insertSynonym("v1", 1);
		mSynonymGroup->insertSynonym("p2", 1);
		mSynonymGroup->insertSynonym("s1", 2);
		mSynonymGroup->insertSynonym("s2", 2);
		mSynonymGroup->insertSynonym("c", 2);

		// Select p1
		ClauseSelectObject selObject = ClauseSelectObject(PROCEDURE, "p1", AttrType::INVALID, false);

		// Modifies_P (p1,v1)
		RelationshipType type1 = MODIFIES_P;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(VARIABLE, std::string("v1"), 0, true);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// with p1.procName = "Second"
		ClauseWithRefObject leftObj1 = ClauseWithRefObject(ATTRREF, PROCEDURE, "p1", AttrType::PROC_NAME);
		ClauseWithRefObject rightObj1 = ClauseWithRefObject(IDENTIFIER, PROCEDURE, "Fourth");
		ClauseWithObject* withThatObj1 = new ClauseWithObject(&leftObj1, &rightObj1);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Calls(p1,p2)
		RelationshipType type = CALLS;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROCEDURE, std::string("p1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROCEDURE, std::string("p2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertSuchThatObject(suchThatObj4);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertWithObject(withThatObj1);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		Assert::IsTrue(0==results.size());
	}

	// Select stmtLst such that Next(n1,n2) and Follows(4,5) and Modifies(s1,"x") with s1.stmt = c.value and Follows(s1,s2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize17) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject n1(PROGRAM_LINE, "n1");
		synonymTable->insert(n1);
		SynonymObject n2(PROGRAM_LINE, "n2");
		synonymTable->insert(n2);
		SynonymObject stmtLst(STMTLST, "stmtLst");
		synonymTable->insert(stmtLst);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("c", 1);
		mSynonymGroup->insertSynonym("n1", 2);
		mSynonymGroup->insertSynonym("n2", 2);

		// Select stmtLst
		ClauseSelectObject selObject = ClauseSelectObject(STMTLST, "stmtLst", AttrType::INVALID, false);

		// Next(n1,n2) 
		RelationshipType type = NEXT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Follows(4,5)
		RelationshipType type1 = FOLLOWS;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(STMT, std::string(""), 5, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,"x") 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		std::vector<std::string> stmtListsValues = { "1", "4", "7", "11", "12", "16" };
		int index = 0;
		for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
			Logger::WriteMessage((*it).c_str());
			Assert::AreEqual(stmtListsValues[index], (*it));
			index++;
		}
	}

	// Select stmtLst such that Next(n1,n2) and Follows(4,6) <-- FALSE and Modifies(s1,"x") with s1.stmt = c.value and Follows(s1,s2)
	TEST_METHOD(TestQueryEvaluator_TestEvaluate_Optimize18) {
		// Initialization
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		SynonymTable *synonymTable = SynonymTable::getInstance();
		synonymTable->clearAll();
		DummyPKB dummyPKB;
		evaluator->setPKB(&dummyPKB);
		SynonymObject s1(STMT, "s1");
		synonymTable->insert(s1);
		SynonymObject s2(STMT, "s2");
		synonymTable->insert(s2);
		SynonymObject c(CONSTANT, "c");
		synonymTable->insert(c);
		SynonymObject n1(PROGRAM_LINE, "n1");
		synonymTable->insert(n1);
		SynonymObject n2(PROGRAM_LINE, "n2");
		synonymTable->insert(n2);
		SynonymObject stmtLst(STMTLST, "stmtLst");
		synonymTable->insert(stmtLst);

		SynonymGroup* mSynonymGroup = SynonymGroup::getInstance();
		mSynonymGroup->insertSynonym("s1", 1);
		mSynonymGroup->insertSynonym("s2", 1);
		mSynonymGroup->insertSynonym("c", 1);
		mSynonymGroup->insertSynonym("n1", 2);
		mSynonymGroup->insertSynonym("n2", 2);

		// Select stmtLst
		ClauseSelectObject selObject = ClauseSelectObject(STMTLST, "stmtLst", AttrType::INVALID, false);

		// Next(n1,n2) 
		RelationshipType type = NEXT;
		ClauseSuchThatArgObject* argOne = new ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n1"), 0, true);
		ClauseSuchThatArgObject* argTwo = new  ClauseSuchThatArgObject(PROGRAM_LINE, std::string("n2"), 0, true);
		ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);

		// Follows(4,5)
		RelationshipType type1 = FOLLOWS;
		ClauseSuchThatArgObject* argOne1 = new ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
		ClauseSuchThatArgObject* argTwo1 = new  ClauseSuchThatArgObject(STMT, std::string(""), 6, false);
		ClauseSuchThatObject* suchThatObj1 = new ClauseSuchThatObject(type1, argOne1, argTwo1);

		// Modifies(s1,"x") 
		RelationshipType type3 = MODIFIES;
		ClauseSuchThatArgObject* argOne3 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo3 = new  ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
		ClauseSuchThatObject* suchThatObj3 = new ClauseSuchThatObject(type3, argOne3, argTwo3);

		// with s1.stmt = c.value
		ClauseWithRefObject leftObj = ClauseWithRefObject(ATTRREF, STMT, "s1", AttrType::STMT_NO);
		ClauseWithRefObject rightObj = ClauseWithRefObject(ATTRREF, CONSTANT, "c", AttrType::VALUE);
		ClauseWithObject* withThatObj = new ClauseWithObject(&leftObj, &rightObj);

		// Follows(s1,s2)
		RelationshipType type4 = FOLLOWS;
		ClauseSuchThatArgObject* argOne4 = new ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
		ClauseSuchThatArgObject* argTwo4 = new  ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
		ClauseSuchThatObject* suchThatObj4 = new ClauseSuchThatObject(type4, argOne4, argTwo4);

		QueryTable queryTable;
		queryTable.insertSelectObject(selObject);
		queryTable.insertSuchThatObject(suchThatObj);
		queryTable.insertSuchThatObject(suchThatObj1);
		queryTable.insertWithObject(withThatObj);
		queryTable.insertSuchThatObject(suchThatObj3);
		queryTable.insertSuchThatObject(suchThatObj4);

		std::vector<std::string> results = evaluator->evaluate(queryTable);
		Assert::IsTrue(0 == results.size());
	}

	// POPULATE SYNONYM GROUP
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
