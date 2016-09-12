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


	};
}