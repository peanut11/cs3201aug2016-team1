#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryValidator) {
public:

	TEST_METHOD(TestQueryValidator_procedure_only) {
		std::string str1 = "procedure p;";
		//str1.append("procedure ").append("p").append(";");

		std::string str2 = "procedure p, q;";
		std::string str3 = "procedure p, q, r;";

		QueryValidator *validator = QueryValidator::getInstance();
	
		Assert::IsTrue(validator->isSynonymDelcaration(str1));
		Assert::IsTrue(validator->isSynonymDelcaration(str2));
		Assert::IsTrue(validator->isSynonymDelcaration(str3));
	}


	};
}