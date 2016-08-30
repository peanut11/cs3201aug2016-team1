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
		std::string str4 = "procedure ;";
		std::string str5 = "procedure p";

		QueryValidator *validator = QueryValidator::getInstance();
	
		Assert::IsTrue(validator->isValidQuery(str1));
		Assert::IsTrue(validator->isValidQuery(str2));
		Assert::IsTrue(validator->isValidQuery(str3));
		Assert::IsFalse(validator->isValidQuery(str4));
		Assert::IsFalse(validator->isValidQuery(str5));
	}


	};
}