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
		Assert::AreEqual(std::string("a"), resultsTable.getObject("a").getSynonym());
		Assert::IsFalse(resultsTable.insert(resultsObj1));
		resultsTable.insert(resultsObj2);
		Assert::AreEqual(std::string("b"), resultsTable.getObject("b").getSynonym());
	}

	TEST_METHOD(TestResultsTable_InsertSet) {
		ResultsTable resultsTable;
		std::set<StmtNumber> statements = { 1,3,5,6,7 };
		std::set<VarName> variables = { "x", "y", "z" };
		ResultsObject resultsObj("a");
		ResultsObject resultsObj1("v"); 
		resultsTable.insert(resultsObj);
		resultsTable.insert(resultsObj1);
		resultsTable.insertSet("a", statements);
		resultsTable.insertSet("v", variables);
		Logger::WriteMessage("hello");
		if (statements == resultsTable.getSetInt("a")) {
			Assert::IsTrue(false);

		}
		if (variables == resultsTable.getSetString("asdasdasdsa")) {
			Assert::IsTrue(true);
		}
	}
	};

}