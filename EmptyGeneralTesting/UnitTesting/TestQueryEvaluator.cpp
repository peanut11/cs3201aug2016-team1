#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/AttrType.h"
#include "../SPA/SelectObject.h"
#include <iostream>
#include <string>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryEvaluator) {

public:
	TEST_METHOD(TestQueryEvaluator_SelectObject) {
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		// 'Select BOOLEAN'
		SelectObject selTrue = SelectObject(CONSTANT, "", AttrType::VAR_NAME, true);
		// 'Select s'
		SelectObject selFalse = SelectObject(CONSTANT, "s", AttrType::VAR_NAME, false);
		// Relationships holds
		Assert::AreEqual(std::string("true"),evaluator->evaluateSelect(selTrue, true)[0]);
		Assert::AreEqual(std::string("2"), evaluator->evaluateSelect(selFalse, true)[1]);
		// Relationships don't holds
		Assert::AreEqual(std::string("false"), evaluator->evaluateSelect(selTrue, false)[0]);
		int resultsSize = evaluator->evaluateSelect(selFalse, false).size();
		Assert::AreEqual(0, resultsSize);

	}
	TEST_METHOD(TestQueryEvaluator_PopulateResultsTable) {
		QueryEvaluator *evaluator = QueryEvaluator::getInstance();
		// assign a, while w, variable v
		SynonymTable *synonymTable = SynonymTable::getInstance();
		SynonymObject a(ASSIGN, "a");
		SynonymObject w(WHILE, "w");
		SynonymObject v(VARIABLE, "v");
		synonymTable->insert(a);
		synonymTable->insert(w);
		synonymTable->insert(v);
		/*		ResultsTable resultsTable = evaluator->populateResultTable(synonymTable);
		Assert::AreEqual(std::string("a"), resultsTable.getObject("a").getSynonym());
		Assert::AreEqual(std::string("w"), resultsTable.getObject("w").getSynonym());
		Assert::AreEqual(std::string("v"), resultsTable.getObject("v").getSynonym());
		*/
	}
	};

}