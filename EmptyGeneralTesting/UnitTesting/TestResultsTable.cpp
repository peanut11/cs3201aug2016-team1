#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ResultsTable.h"
#include "../SPA/ResultsObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestResultsTable) {

public:
	TEST_METHOD(TestResultsTable_InsertResultsObject) {
		ResultsTable resultsTable;
		ResultsObject resultsObj("a");
		ResultsObject resultsObj1("a");
		ResultsObject resultsObj2("b");
		resultsTable.insert(resultsObj);
		Assert::AreEqual(std::string("a"), resultsTable.getObject("a")->getSynonym());
		Assert::IsFalse(resultsTable.insert(resultsObj1));
		resultsTable.insert(resultsObj2);
		Assert::AreEqual(std::string("b"), resultsTable.getObject("b")->getSynonym());
		Assert::AreEqual(std::string("INVALID"), resultsTable.getObject("asdasdadasd")->getSynonym());
	}
	TEST_METHOD(TestResultsTable_InsertSet) {
		ResultsTable resultsTable;
		std::set<StmtNumber> statements = { 1,3,5,6,7 };
		std::set<VarName> variables = { "x", "y", "z" };
		ResultsObject resultsObj = ResultsObject("a");
		ResultsObject resultsObj1 = ResultsObject("v");
		resultsTable.insert(resultsObj);
		resultsTable.insert(resultsObj1);

		if (resultsTable.contains("a")) {
			Logger::WriteMessage("a is inserted");
		}
		if (resultsTable.contains("v")) {
			Logger::WriteMessage("v is inserted");
		}
		if (resultsTable.insertSet("a", statements)) {
			Logger::WriteMessage("set is inserted into 'a'");
		}
		if (resultsTable.insertSet("v", variables)) {
			Logger::WriteMessage("set is inserted into 'v'");
		}

		Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());
		Logger::WriteMessage(std::to_string(resultsTable.getSetString("v").size()).c_str());
		Assert::AreEqual(std::to_string(statements.size()), std::to_string(resultsTable.getSetInt("a").size()));
		Assert::AreEqual(std::to_string(variables.size()), std::to_string(resultsTable.getSetString("v").size()));
	}
	TEST_METHOD(TestResultsTable_ReplaceSet) {
		ResultsTable resultsTable;
		std::set<StmtNumber> statements = { 1,3,5,6,7 };
		std::set<VarName> variables = { "x", "y", "z" };
		std::set<StmtNumber> Rstatements = { 2,4,7,8 };
		std::set<VarName> Rvariables = { "s", "p" };
		ResultsObject resultsObj = ResultsObject("a");
		ResultsObject resultsObj1 = ResultsObject("v");
		resultsTable.insert(resultsObj);
		resultsTable.insert(resultsObj1);

		if (resultsTable.contains("a")) {
			Logger::WriteMessage("a is inserted");
		}
		if (resultsTable.contains("v")) {
			Logger::WriteMessage("v is inserted");
		}
		if (resultsTable.insertSet("a", statements)) {
			Logger::WriteMessage("set is inserted into 'a'");
		}
		if (resultsTable.insertSet("v", variables)) {
			Logger::WriteMessage("set is inserted into 'v'");
		}

		Assert::AreEqual(std::to_string(statements.size()), std::to_string(resultsTable.getSetInt("a").size()));
		Assert::AreEqual(std::to_string(variables.size()), std::to_string(resultsTable.getSetString("v").size()));

		Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());

		resultsTable.replaceSet("a", Rstatements);
		Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());

		resultsTable.replaceSet("v", Rvariables);
		Logger::WriteMessage(std::to_string(resultsTable.getSetString("v").size()).c_str());

		Assert::AreEqual(std::to_string(Rstatements.size()), std::to_string(resultsTable.getSetInt("a").size()));
		Assert::AreEqual(std::to_string(Rvariables.size()), std::to_string(resultsTable.getSetString("v").size()));
	}
	TEST_METHOD(TestResultsTable_GetObjects) {
		ResultsTable resultsTable;
		ResultsObject resultsObj("a");
		ResultsObject resultsObj1("a");
		ResultsObject resultsObj2("b");
		Assert::AreEqual(std::to_string(0), std::to_string(resultsTable.getObjects().size()));
		resultsTable.insert(resultsObj);
		resultsTable.insert(resultsObj1);
		resultsTable.insert(resultsObj2);
		Assert::AreEqual(std::to_string(2), std::to_string(resultsTable.getObjects().size()));
	}
	};

}