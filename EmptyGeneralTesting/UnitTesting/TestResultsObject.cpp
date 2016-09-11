#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ResultsTable.h"
#include "../SPA/ResultsObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestResultsObject) {

public:
	TEST_METHOD(TestResultsObject_InsertSet) {
		ResultsObject r = ResultsObject("a");
		Logger::WriteMessage("hello");
		ResultsObject v(std::string("v"));

		std::set<StmtNumber> statements = { 1,3,5,6,7 };
		std::set<VarName> variables = { "x", "y", "z" };
		r.insertSet(statements);
		v.insertSet(variables);
		Logger::WriteMessage(std::to_string(r.getSetInt().size()).c_str());
		Logger::WriteMessage(std::to_string(v.getSetString().size()).c_str());
	}
	};

}