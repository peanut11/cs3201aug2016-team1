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
				Logger::WriteMessage(resultsObj.getSynonym().c_str());
		Logger::WriteMessage("haha");
		resultsTable.getObject("a").getSynonym();
		Logger::WriteMessage("haha");
		Assert::AreEqual(std::string("a"), resultsTable.getObject("a").getSynonym());
		Assert::IsFalse(resultsTable.insert(resultsObj1));
		resultsTable.insert(resultsObj2);
		Assert::AreEqual(std::string("b"), resultsTable.getObject("b").getSynonym());
		Assert::AreEqual(std::string("INVALID"), resultsTable.getObject("asdasdadasd").getSynonym());
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
			Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());
		}
		if (resultsTable.insertSet("v", variables)) {
			Logger::WriteMessage("set is inserted into 'v'");
		}
//		ResultsObject *ptr = &resultsTable.getObject("a");
//		ptr->insertSet(statements);
		Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());
		Logger::WriteMessage(std::to_string(resultsTable.getObject("a").getSetInt().size()).c_str());
/*
		ResultsObject *rex = &resultsTable.getObject("a");
		rex->insertSet(statements);
		Logger::WriteMessage(std::to_string(rex->getSetInt().size()).c_str());
		Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());
		if (resultsTable.insertSet("a", statements)) {
			Logger::WriteMessage("set is inserted into 'a'");
			Logger::WriteMessage(std::to_string(resultsTable.getObject("a").getSetInt().size()).c_str());
			Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());
		}
		if (resultsTable.insertSet("v", variables)) {
			Logger::WriteMessage("set is inserted into 'v'");
		}
		ResultsObject haha = resultsTable.getObject("a");
		Logger::WriteMessage(haha.getSynonym().c_str());
		Logger::WriteMessage(std::to_string(haha.getSetInt().size()).c_str());
		Logger::WriteMessage(std::to_string(haha.getSetString().size()).c_str());

		//		StmtNumber x = *std::next(statements.begin(), 1);
		//		haha.insertSet(variables);
		//		haha.insertSet(statements);
		if (resultsTable.getObject("a").getSetInt().size() == 0) {

		}
		std::set<StmtNumber> k = resultsTable.getObject("a").getSetInt();
		StmtNumber x = *std::next(k.begin(), 0);
		Logger::WriteMessage(std::to_string(x).c_str());



		Logger::WriteMessage(std::to_string(resultsTable.getObject("a").getSetInt().size()).c_str());
		Logger::WriteMessage(std::to_string(resultsTable.getSetInt("a").size()).c_str());
		Logger::WriteMessage(std::to_string(resultsTable.getSetString("v").size()).c_str());
		for (int i = 0; i < 4; i++) {
			StmtNumber x = *std::next(statements.begin(), i);
			Logger::WriteMessage(std::to_string(x).c_str());
		}
		if (variables == resultsTable.getSetString("asdasdasdsa")) {

		}
*/
	}
	};

}