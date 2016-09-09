#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/AttrType.h"
#include "../SPA/SelectObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryEvaluator) {

public:
		TEST_METHOD(TestQueryEvaluator_SelectObject) {
			QueryEvaluator *evaluator = QueryEvaluator::getInstance();
			SelectObject sel = SelectObject(CONSTANT, AttrType::VAR_NAME, true);
			Assert::IsTrue(evaluator->evaluateSelect(sel,true));
		}
	};

}