#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryProcessor.h"
#include "../SPA/QueryPreProcessor.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/RelTable.h"
#include "../SPA/RelObject.h"
#include "../SPA/RelObjectType.h"
#include "../SPA/RelArgsType.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryProcessor)
	{
	public:

		TEST_METHOD(TestQueryProcessor_init){

			QueryProcessor *processor = QueryProcessor::getInstance();

			int currentRelSize = processor->getQueryPreProcessor()->getRelationshipTable()->size();
			Assert::AreEqual(currentRelSize, 1);

			int currentSynonymSize = processor->getQueryPreProcessor()->getSynonymTable()->size();
			Assert::AreEqual(currentSynonymSize, 2);

		}

	};
}