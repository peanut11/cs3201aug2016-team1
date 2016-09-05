#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryValidator.h"
#include "../SPA/QueryProcessor.h"

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

		// Success test cases
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a;\nSelect p"));
		Assert::AreEqual(validator->getSynonymTable()->size(), 2);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();
		/*
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a;\nSelect p.procName"));
		Assert::AreEqual(validator->getSynonymTable()->size(), 2);
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		validator->clearSynonymTable();
		*/
		// Failed test cases
		Assert::IsFalse(validator->isValidQuery("procedure p;assign a;\nSelect"));
		validator->clearSynonymTable();

		// if ifstmt;whilew;\nSelect w such that Parent(ifstmt, w)

	}


	TEST_METHOD(TestQueryValidator_Check_tokenizer) {
		/*
		StringTokenizer st = StringTokenizer("procedure p;assign a;\nSelect p.procName", QUERY_PREPROCESSOR);

		Assert::AreEqual(std::string("procedure"), st.nextToken());
		Assert::AreEqual(std::string("p"), st.nextToken());
		Assert::AreEqual(std::string(";"), st.nextToken());
		Assert::AreEqual(std::string("assign"), st.nextToken());
		Assert::AreEqual(std::string("a"), st.nextToken());
		Assert::AreEqual(std::string(";"), st.nextToken());
		Assert::AreEqual(std::string("\n"), st.nextToken());
		Assert::AreEqual(std::string("Select"), st.nextToken());
		Assert::AreEqual(std::string("p"), st.nextToken());
		Assert::AreEqual(std::string("."), st.nextToken());
		Assert::AreEqual(std::string("procName"), st.nextToken());
		*/

		StringTokenizer st = StringTokenizer("1,2", QUERY_PREPROCESSOR);
		Assert::AreEqual(std::string("1"), st.nextToken());
		Assert::AreEqual(std::string(","), st.nextToken());
		Assert::AreEqual(std::string("2"), st.nextToken());

		st = StringTokenizer("Parent(1,2)", QUERY_PREPROCESSOR);
		Assert::AreEqual(std::string("Parent"), st.nextToken());
		Assert::AreEqual(std::string("("), st.nextToken());
		Assert::AreEqual(std::string("1"), st.nextToken());
		Assert::AreEqual(std::string(","), st.nextToken());
		Assert::AreEqual(std::string("2"), st.nextToken());
		Assert::AreEqual(std::string(")"), st.nextToken());
	}

	TEST_METHOD(TestQueryValidator_Variable_Only) {
		QueryValidator *validator = QueryValidator::getInstance();
		// success
		validator->initStringTokenizer("\"a\"");
		Assert::IsTrue(validator->isVariable("\""));

		// failure
		validator->initStringTokenizer("\"a"); // no back "
		Assert::IsFalse(validator->isVariable("\""));

		validator->initStringTokenizer("a\""); // no front "
		Assert::IsFalse(validator->isVariable(""));

		validator->initStringTokenizer("a"); // no front and back "
		Assert::IsFalse(validator->isVariable(""));

		validator->initStringTokenizer("\"\""); // no name inside
		Assert::IsFalse(validator->isVariable("\""));

	}


	TEST_METHOD(TestQueryValidator_Argument_Only) {

		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();
		
		Assert::IsTrue(validator->isStatementNumber("1"));
		Assert::IsTrue(validator->isStatementNumber("2"));
		Assert::IsTrue(validator->isSynonym("var1"));
		Assert::IsFalse(validator->isSynonym("1"));
		Assert::IsFalse(validator->isStatementNumber("p"));

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;\nSelect p"));
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());


		// success
		validator->initStringTokenizer("(1,2)"); // stmt numbers
		Assert::IsTrue(validator->isArguments("(", validator->getRelationshipTable()->find(RelationshipType::PARENT))); // Parent
		
		validator->initStringTokenizer("(p,p)");
		Assert::IsTrue(validator->isArguments("(", validator->getRelationshipTable()->find(RelationshipType::CALLS))); // Calls

		validator->initStringTokenizer("(p,var1)");
		Assert::IsTrue(validator->isArguments("(", validator->getRelationshipTable()->find(RelationshipType::MODIFIES))); // Modifies


		// failure
		validator->initStringTokenizer("(1,)"); // no second arg, with comma
		Assert::IsFalse(validator->isArguments("(", validator->getRelationshipTable()->getObject(4)));
	
		validator->initStringTokenizer("(1)"); // only 1 arg, no comma
		Assert::IsFalse(validator->isArguments("(", validator->getRelationshipTable()->getObject(4)));

		validator->initStringTokenizer("(,2)"); // no first arg, with comma
		Assert::IsFalse(validator->isArguments("(", validator->getRelationshipTable()->getObject(4)));

		validator->initStringTokenizer("p,p)"); // parent should have stmt args, but both args are procedure
		Assert::IsFalse(validator->isArguments("p", validator->getRelationshipTable()->find(RelationshipType::PARENT))); // Parent can only have stmt

		validator->initStringTokenizer("p,1)"); // parent should have stmt args, but both args are procedure
		Assert::IsFalse(validator->isArguments("p", validator->getRelationshipTable()->find(RelationshipType::PARENT))); // Parent can only have stmt


	}

	TEST_METHOD(TestQueryValidator_Relationship) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		//std::string procString = "procedure";
		//Assert::AreEqual(validator->getSynonymTable()->getObject("p").getTypeString().c_str(), procString.c_str());

		// success
		validator->initStringTokenizer("Parent(1,2)"); // parent
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,s2)"); // parent
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(1,s2)"); // parent
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,2)"); // parent
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Follows(s1,2)"); // follows
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Modifies(p,var1)"); // modifies
		Assert::IsTrue(validator->isRelationship("Modifies"));


		// failure
		validator->initStringTokenizer("Parent(p,q)"); // parent should have stmt args, but both args are procedure
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,q)"); // parent
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(p,s2)"); // parent
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1)"); // parent
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,)"); // parent
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Follows(p,q)"); // follows
		Assert::IsFalse(validator->isRelationship("Follows"));

	}

	};
}