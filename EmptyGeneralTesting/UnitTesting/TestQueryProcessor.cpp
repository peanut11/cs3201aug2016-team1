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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryProcessor)
	{
	public:

		std::string declaration = "procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\n";

		TEST_METHOD(TestQueryProcessor_Init){
			QueryProcessor *processor = QueryProcessor::getInstance();
		}

		TEST_METHOD(TestQueryProcessor_Valid_Query) {

			QueryProcessor *processor = QueryProcessor::getInstance();

			//Assert::IsTrue(processor->evaluate(declaration + "Select p such that Parent(s1,s2)"));

		}

		TEST_METHOD(TestQueryProcessor_Modifies) {
			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			DummyPKB dummyPKB;
			queryProcessor->getQueryEvaluator()->setPKB(&dummyPKB);
			std::string declaration = "assign a1;stmt s;\n";
			std::vector<std::string> results = queryProcessor->evaluate(declaration + "Select s such that Modifies(s,\"x\")");
			for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
				Logger::WriteMessage((*it).c_str());
			}
		}
		TEST_METHOD(TestQueryProcessor_ModifiesPattern) {
			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			DummyPKB dummyPKB;
			queryProcessor->getQueryEvaluator()->setPKB(&dummyPKB);
			std::string declaration = "assign a1;stmt s;variable v1;\n";
			std::vector<std::string> results = queryProcessor->evaluate(declaration + "Select s such that Modifies(s,\"x\") and pattern a1(v1, _\"x\"_");
			for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
				Logger::WriteMessage((*it).c_str());
			}
		}
		TEST_METHOD(TestQueryProcessor_ModifiesPattern1) {
			QueryProcessor *queryProcessor = QueryProcessor::getInstance();
			DummyPKB dummyPKB;
			queryProcessor->getQueryEvaluator()->setPKB(&dummyPKB);
			std::string declaration = "assign a1;stmt s;variable v1;\n";
			std::vector<std::string> results = queryProcessor->evaluate(declaration + "Select s such that Modifies(s,\"x\") and pattern a1(v1, _)");
			for (std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
				Logger::WriteMessage((*it).c_str());
			}
		}
	};
}