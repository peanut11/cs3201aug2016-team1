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

	/*
    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow1) {
        // FOLLOWS(3,4) = TRUE
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
        Logger::WriteMessage(std::to_string(argOne.getIntegerValue()).c_str());
        Logger::WriteMessage(std::to_string(argTwo.getIntegerValue()).c_str());
        ClauseSuchThatObject* suchThatObj = new ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject* resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj->getResultsBoolean());

        ClauseSuchThatArgObject arg3 = ClauseSuchThatArgObject(STMT, std::string(""), 1, false);
        ClauseSuchThatArgObject arg4 = ClauseSuchThatArgObject(STMT, std::string(""), 4, false);
        ClauseSuchThatObject* st1 = new ClauseSuchThatObject(type, arg3, arg4);
        ClauseSuchThatObject* re1 = evaluator->evaluateSuchThat(st1);
        Assert::IsFalse(re1->getResultsBoolean());

        // SELECT BOOLEAN such that Follows(3,4)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());

        // SELECT s such that Follows(3,4)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow2) {
        // Follows(s,3)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s,3)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s,3)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow3) {
        // Follows(a,3)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(ASSIGN, std::string("a"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(a,3)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Follows(a,3)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow4) {
        //  Follows(3,s)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(3,s)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(3,s)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(3).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow5) {
        //  Follows(3,a)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(ASSIGN, std::string("a"), 0, true);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(3,a)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Follows(3,a)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow6) {
        //  Follows(_,10);
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string(""), 10, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(_,10);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(_,10);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow7) {
        //  Follows(_,s);
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(_,s);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(_,s);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow8) {
        //  Follows(3,_);
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        // SELECT BOOLEAN such that Follows(3,_);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(3,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow9) {
        //  Follows(s,_);
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s,_);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Follow10) {
        //  Follows(s1,s2); Follow(a,s1);
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
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s1");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");
        std::set<StmtNumber> current1 = resultManager->getValuesForSynonym("s2");
        for (std::set<StmtNumber>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current1.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = FOLLOWS;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("s1"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string("s2"), 0, true);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());
        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s1");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());
        Logger::WriteMessage("=============");
        std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("s2");
        for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that Follows(s1,s2)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());

        Logger::WriteMessage("=============");

        // SELECT s1 such that Follows(s1,s2)
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s1", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT s2 such that Follows(s1,s2)
        ClauseSelectObject selObject2 = ClauseSelectObject(STMT, "s2", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT s such that Follows(s1,s2)
        ClauseSelectObject selObject3 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results3 = evaluator->evaluateSelect(selObject3, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results3.begin(); it != results3.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(results3.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies1) {
        // (Modifies(3,"x"))
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        ClauseSuchThatArgObject arg3 = ClauseSuchThatArgObject(STMT, std::string("y"), 0, false);
        ClauseSuchThatObject st1 = ClauseSuchThatObject(type, argOne, arg3);
        ClauseSuchThatObject re1 = evaluator->evaluateSuchThat(st1);
        Assert::IsFalse(re1.getResultsBoolean());

        // SELECT BOOLEAN such that (Modifies(3,"x"))
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(3,"x"))
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(9).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies2) {
        // (Modifies(3,v))
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<VarName> current = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());
        std::set<VarName> updated = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(3,v))
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that (Modifies(3,v))
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies3) {
        // (Modifies(3,_)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject v(VARIABLE, "v");
        synonymTable->insert(v);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<VarName> current = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(current.size()).c_str());
        Logger::WriteMessage("=============");

        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 3, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(VARIABLE, std::string("_"), 0, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(3,_)
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that (Modifies(3,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies4) {
        // (Modifies(s,"x")
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(VARIABLE, std::string("x"), 0, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(s,"x")
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(s,"x")
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies5) {
        // (Modifies(s,_);
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject s(STMT, "s");
        synonymTable->insert(s);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string("_"), 0, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(updated.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT BOOLEAN such that (Modifies(s,_);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that (Modifies(s,_);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results1.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluateSuchThat_Modifies6) {
        // Modifies(s,v);
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
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        std::set<VarName> current1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = current1.begin(); it != current1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");

        RelationshipType type = MODIFIES;
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string("s"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("s");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(updated.size()).c_str());

        std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Modifies(s,v);
        ClauseSelectObject selObject = ClauseSelectObject(CONSTANT, "", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT s such that Modifies(s,v);
        ClauseSelectObject selObject1 = ClauseSelectObject(STMT, "s", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(4).c_str(), std::to_string(results1.size()).c_str());

        Logger::WriteMessage("=============");

        // SELECT v such that Modifies(s,v);
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern1) {
        // Pattern a(v,_)
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

        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "v";
        std::string secondArg = "_";
        bool isFirstArgSynonym = true;
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated.size()).c_str());

        std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Pattern a(v,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(v,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that Pattern a(v,_)
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(2).c_str(), std::to_string(results2.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern2) {
        // Pattern a(_,_)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        EntityType patternType = ASSIGN;
        EntityType firstArgType = WILDCARD;
        std::string patternSynonymArg = "a";
        std::string firstArg = "_";
        std::string secondArg = "_";
        bool isFirstArgSynonym = false;
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        // SELECT BOOLEAN such that Pattern a(_,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(_,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(5).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern3) {
        // Pattern a("x",_)
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "x";
        std::string secondArg = "_";
        bool isFirstArgSynonym = false;
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that Pattern a("x",_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a("x",_)
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern4) {
        // Pattern a(v,"_<constant/variable>_")
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

        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "v";
        std::string secondArg = "_z_";
        bool isFirstArgSynonym = true;
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        std::set<VarName> updated1 = evaluator->getValuesForSynonym("v");
        for (std::set<VarName>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that Pattern a(v,"_<constant/variable>_")
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(v,"_<constant/variable>_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(v,"_<constant/variable>_")
        ClauseSelectObject selObject2 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results2 = evaluator->evaluateSelect(selObject2, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results2.begin(); it != results2.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results2.size()).c_str());
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern5) {
        // Pattern a(_, "_<constant/variable>_")
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        EntityType patternType = ASSIGN;
        EntityType firstArgType = INVALID;
        std::string patternSynonymArg = "a";
        std::string firstArg = "_";
        std::string secondArg = "_z_";
        bool isFirstArgSynonym = false;
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that a(_, "_<constant/variable>_")
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a(_, "_<constant/variable>_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }

    TEST_METHOD(TestQueryEvaluator_TestEvaluatePattern6) {
        // Pattern a("x","_<constant/variable>_")
        QueryEvaluator *evaluator = QueryEvaluator::getInstance();
        SynonymTable *synonymTable = SynonymTable::getInstance();
        synonymTable->clearAll();
        DummyPKB dummyPKB;
        evaluator->setPKB(&dummyPKB);
        SynonymObject a(ASSIGN, "a");
        synonymTable->insert(a);
        ResultGridManager* resultManager = evaluator->populateResultGrids();
        std::set<StmtNumber> current = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = current.begin(); it != current.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");

        EntityType patternType = ASSIGN;
        EntityType firstArgType = VARIABLE;
        std::string patternSynonymArg = "a";
        std::string firstArg = "x";
        std::string secondArg = "_z_";
        bool isFirstArgSynonym = false;
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj.getResultsBoolean());

        std::set<StmtNumber> updated = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated.begin(); it != updated.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that Pattern a("x","_<constant/variable>_")
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that Pattern a("x","_<constant/variable>_")
        ClauseSelectObject selObject1 = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
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
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(ASSIGN, std::string("a"), 0, true);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(STMT, std::string(""), 7, false);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());
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
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj1 = evaluator->evaluatePattern(patternObj);
        Assert::IsFalse(resultObj1.getResultsBoolean());
        Logger::WriteMessage("====asdasdsad======");

        std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated.size()).c_str());

        // SELECT BOOLEAN such that  Follow (a,8) and Pattern a(v,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean() && resultObj1.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("false", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT a such that  Follow (a,8) and Pattern a(v,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
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
        ClauseSuchThatArgObject argOne = ClauseSuchThatArgObject(STMT, std::string(""), 8, false);
        ClauseSuchThatArgObject argTwo = ClauseSuchThatArgObject(VARIABLE, std::string("v"), 0, true);
        ClauseSuchThatObject suchThatObj = ClauseSuchThatObject(type, argOne, argTwo);
        ClauseSuchThatObject resultObj = evaluator->evaluateSuchThat(suchThatObj);
        Assert::IsTrue(resultObj.getResultsBoolean());
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
        ClausePatternObject patternObj = ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
        ClausePatternObject resultObj1 = evaluator->evaluatePattern(patternObj);
        Assert::IsTrue(resultObj1.getResultsBoolean());
        std::set<StmtNumber> updated1 = resultManager->getValuesForSynonym("a");
        for (std::set<StmtNumber>::iterator it = updated1.begin(); it != updated1.end(); ++it) {
            Logger::WriteMessage(std::to_string(*it).c_str());
        }
        Logger::WriteMessage("=============");
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(updated1.size()).c_str());

        // SELECT BOOLEAN such that  Modifies(8,v)  and Pattern a(v,_)
        ClauseSelectObject selObject = ClauseSelectObject(ASSIGN, "a", AttrType::INVALID, true);
        std::vector<std::string> results = evaluator->evaluateSelect(selObject, resultObj.getResultsBoolean() && resultObj1.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual("true", results.begin()->c_str());
        Logger::WriteMessage("=============");

        // SELECT v such that  Modifies(8,v)  and Pattern a(v,_)
        ClauseSelectObject selObject1 = ClauseSelectObject(VARIABLE, "v", AttrType::INVALID, false);
        std::vector<std::string> results1 = evaluator->evaluateSelect(selObject1, resultObj.getResultsBoolean());
        for (std::vector<std::string>::iterator it = results1.begin(); it != results1.end(); ++it) {
            Logger::WriteMessage((*it).c_str());
        }
        Assert::AreEqual(std::to_string(1).c_str(), std::to_string(results1.size()).c_str());
        Logger::WriteMessage("=============");
    }
    
	*/


};

}
