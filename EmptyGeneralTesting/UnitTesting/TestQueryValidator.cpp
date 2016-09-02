#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryValidator) {
public:
	
	TEST_METHOD(TestQueryValidator_Procedure_Only) {
		std::string str1 = "procedure p;\n";

		std::string str11 = "procedure p, q;\n";
		std::string str12 = "procedure p;procedure q;\n";
		std::string str13 = "procedure p;procedure q;procedure r;\n";
		std::string str14 = "procedure p;procedure p;procedure r;\n"; // try insert two procedure p

		std::string str21 = "procedure"; // no synonym & semi-colon
		std::string str22 = "procedure ;"; // no synonym
		std::string str23 = "procedure p, ;"; // no next synonym
		std::string str24 = "procedure p, "; // no next synonym & semi-colon
		std::string str25 = "proc "; // no next synonym & semi-colon
		std::string str26 = "procedure p;"; // no \n
		std::string str27 = "Procedure p;"; // capital letter (first)

		QueryValidator *validator = QueryValidator::getInstance();
	
		Assert::IsTrue(validator->isValidQuery(str1));
		Assert::AreEqual(validator->getSynonymTable()->size(), 1);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();
		
		Assert::IsTrue(validator->isValidQuery(str11));
		Assert::AreEqual(validator->getSynonymTable()->size(), 2);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();
		
		Assert::IsTrue(validator->isValidQuery(str12));
		Assert::AreEqual(validator->getSynonymTable()->size(), 2);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();

		Assert::IsTrue(validator->isValidQuery(str13));
		Assert::AreEqual(validator->getSynonymTable()->size(), 3);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();

		auto funcPtr = [validator] { validator->isValidQuery("procedure p;procedure p;procedure r;\n"); };
		Assert::ExpectException<std::runtime_error>(funcPtr); // try insert two procedure p, return exception
		validator->clearSynonymTable();

		Assert::IsFalse(validator->isValidQuery(str21));
		validator->clearSynonymTable();
		Assert::IsFalse(validator->isValidQuery(str22));
		validator->clearSynonymTable();
		Assert::IsFalse(validator->isValidQuery(str23));
		validator->clearSynonymTable();
		Assert::IsFalse(validator->isValidQuery(str24));
		validator->clearSynonymTable();
		Assert::IsFalse(validator->isValidQuery(str25));
		validator->clearSynonymTable();
		Assert::IsFalse(validator->isValidQuery(str26));
		validator->clearSynonymTable();
		Assert::IsFalse(validator->isValidQuery(str27));
		validator->clearSynonymTable();
	}

	TEST_METHOD(TestQueryValidator_Mix_Synonyms) {
		QueryValidator *validator = QueryValidator::getInstance();
	
		std::string str1 = "procedure p; assign a;\n";
		Assert::IsTrue(validator->isValidQuery(str1));
		Assert::AreEqual(validator->getSynonymTable()->size(), 2);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();

		std::string str2 = "assign a; procedure p;\n";
		Assert::IsTrue(validator->isValidQuery(str2));
		Assert::AreEqual(validator->getSynonymTable()->size(), 2);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();
		
		std::string str3 = "assign a; procedure p;if ifstmt, ifstmt2;\n";
		Assert::IsTrue(validator->isValidQuery(str3));
		Assert::AreEqual(validator->getSynonymTable()->size(), 4);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();

	}
	

	TEST_METHOD(TestQueryValidator_Select_Only) {
		QueryValidator *validator = QueryValidator::getInstance();

		std::string str1 = "procedure p;assign a;\nSelect p";
		Assert::IsTrue(validator->isValidQuery(str1));
		Assert::AreEqual("Select", validator->getNextToken().c_str());
		Assert::AreEqual("p", validator->getNextToken().c_str());
		Assert::AreEqual(validator->getSynonymTable()->size(), 2);



		//Assert::IsFalse(validator->isDeclaredSynonym("r"));
		//Assert::IsTrue(validator->isDeclaredSynonym("p"));
		//Assert::IsTrue(validator->isDeclaredSynonym("a"));
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		
		validator->clearSynonymTable();

	}

	};
}