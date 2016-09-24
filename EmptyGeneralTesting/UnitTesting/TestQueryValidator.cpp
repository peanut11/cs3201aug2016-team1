#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryValidator.h"
#include "../SPA/QueryProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestQueryValidator) {
public:
	
	TEST_METHOD(TestQueryValidator__Iteration_2_Full_Query) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;prog_line n1, n2;\n";

		// Success Next
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(_,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(_,_)"));

		// Success Next*
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(n1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(n1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(_,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(n1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(_,_)"));
		

		// Success Affects
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(n1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(n1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(a1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(1,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(a1,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(n1,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(a1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(_,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(n1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(_,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(a1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects(_,_)"));
		
		// Success Affects*
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(n1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(n1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(a1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(1,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(a1,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(n1,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(a1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(_,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(n1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(_,a2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(a1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Affects*(_,_)"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());





		// Success With
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with s1.stmt# = 1")); // 1 with clause
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) pattern a1(\"x\",_\"y+1\"_) with s1.stmt# = 1 s2.stmt# = 4")); // 2 with clauses
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with s1.stmt# = 1 such that Follows(s1, _)"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());


		// Failure
		auto error1 = [validator] { 
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;prog_line n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Next(s1,_)");
		};
		Assert::ExpectException<std::runtime_error>(error1);
		
		auto error2 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;prog_line n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Next(_,s2)");
		};
		Assert::ExpectException<std::runtime_error>(error2);

		auto error3 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;prog_line n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Next(p,s2)");
		};
		Assert::ExpectException<std::runtime_error>(error3);

		auto error4 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;prog_line n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Affects(s1,_)");
		};
		Assert::ExpectException<std::runtime_error>(error4);

		auto error5 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;prog_line n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Affects(_,s2)");
		};
		Assert::ExpectException<std::runtime_error>(error5);

		auto error6 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;prog_line n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Affects(s1,p)");
		};
		Assert::ExpectException<std::runtime_error>(error6);

	}

	TEST_METHOD(TestQueryValidator__Iteration_1_Full_Query) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		std::string declaration = "procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\n";

		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;Select p such that Parent(s1,s2)"));

		// Success PARENT
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent(3,s1)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(_,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(_,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) pattern a1(\"x\",_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,_) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(_,s1) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(_,_) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(_,_) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(_,_) pattern a1(_,_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) pattern a1(\"x\",\"y\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(_,_) pattern a1(\"x\", \"y+1\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) pattern a1(\"x\",_\"y+1\"_)"));

		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Parent(s1, s2) Uses(a1, \"x\") pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Parent*(s1, s2) Uses(a1, \"x\") pattern a1(\"x\",_\"y\"_)"));
		
		// success FOLLOWS
		Assert::IsTrue(validator->isValidQuery(declaration + "Select ifstmt such that Follows (5, ifstmt)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows (s1, 3)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows (s1, 3) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(s1, 2) Parent(s1, s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(s1, _) Parent(s1, s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(_, _) Parent(_, s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(_, _) Parent(_, s2) pattern a1(_,_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(_, _) Parent(_, s2) pattern a1(_,_\"y+1\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(_, _) Parent(_, s2) pattern a1(_,_\"1+y+x\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(_, _) Parent(_, s2) pattern a1(_,\"1+y+x\")"));
		//Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows(_, _) Parent(_, s2) pattern a1(\"x\",_)"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(_, _) Parent(_, s2) pattern a1(_,_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(2, s1)"));
		//Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());


		// success USES
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Uses(a1, \"x\")"));
		// 1 common synonym between clauses
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(a1, \"x\") pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(a1, \"x\") pattern a1(\"x\",_\"y+1\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(a1, \"x\") pattern a1(\"x\",\"y+1\")"));
		Assert::IsTrue(validator->isValidQuery("assign a, a1; variable v;\nSelect a such that Uses(a,v) pattern a1(v,_)"));
		
		//Logger::WriteMessage(validator->getSynonymOccurence()->toString().c_str());
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());
		


		// success MODIFIES
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Modifies(a1, \"x\")"));
		// 1 common synonym between clauses
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Modifies(a1, \"x\") pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Modifies(a1, \"x\") pattern a1(\"x\",_\"y+1\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Modifies(a1, \"x\") pattern a1(\"x\",\"y+1\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Uses(a1, \"x\") pattern a1(\"x\",\"y\")"));
		//Logger::WriteMessage(validator->getSynonymOccurence()->toString().c_str());

		Assert::IsTrue(validator->isValidQuery("assign a1;Select a1 pattern a1(_,_\"y\"_)"));
		

		
		// Failed test cases
		// wrong expression


		auto funcPtrExprError1 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",_\"+1\"_)"); };
		Assert::ExpectException<std::runtime_error>(funcPtrExprError1);

		auto funcPtrExprError2 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",_\"+1\")"); };
		Assert::ExpectException<std::runtime_error>(funcPtrExprError2);

		auto funcPtrExprError3 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",\"+1\"_)"); };
		Assert::ExpectException<std::runtime_error>(funcPtrExprError3);

		auto funcPtrExprError4 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",\"_\")"); };
		Assert::ExpectException<std::runtime_error>(funcPtrExprError4);

		auto funcPtrExprError5 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",y)"); };
		Assert::ExpectException<std::runtime_error>(funcPtrExprError5);

		auto funcPtrExprError6 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(0,s1)"); };
		Assert::ExpectException<std::runtime_error>(funcPtrExprError6);

		auto funcPtrExprError7 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1,0)"); };
		Assert::ExpectException<std::runtime_error>(funcPtrExprError7);



		// more than 1 common synonym between clauses
		auto funcPtrError1 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Follows(s1, s2) Parent(s1, s2) pattern a1(\"x\",\"y\")"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError1);
		
		auto funcPtrError2 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;variable v\nSelect a such that Uses (a1, v) pattern a1(v, _)"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError2);



		// USES and MODIFIES cannot have wildcard as first argument
		auto funcPtrWildcardError1 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) Modifies(_, \"x\") pattern a1(\"x\",\"y\")"); };
		Assert::ExpectException<std::runtime_error>(funcPtrWildcardError1);

		auto funcPtrWildcardError2 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) Uses(_, \"x\") pattern a1(\"x\",\"y\")"); };
		Assert::ExpectException<std::runtime_error>(funcPtrWildcardError2);


	}

	/*
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
	*/

	TEST_METHOD(TestQueryValidator_Check_tokenizer) {
		
		StringTokenizer st = StringTokenizer("Select p such that Parent(1,2)", QUERY_PREPROCESSOR);
		Assert::AreEqual(std::string("Select"), st.nextToken());
		Assert::AreEqual(std::string("p"), st.nextToken());
		Assert::AreEqual(std::string("such"), st.nextToken());
		Assert::AreEqual(std::string("that"), st.nextToken());
		Assert::AreEqual(std::string("Parent"), st.nextToken());
		Assert::AreEqual(std::string("("), st.nextToken());
		Assert::AreEqual(std::string("1"), st.nextToken());
		Assert::AreEqual(std::string(","), st.nextToken());
		Assert::AreEqual(std::string("2"), st.nextToken());
		Assert::AreEqual(std::string(")"), st.nextToken());


		st = StringTokenizer("1,2", QUERY_PREPROCESSOR);
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

		st = StringTokenizer("pattern(\"x\",\"x+y\")", QUERY_PREPROCESSOR);
		Assert::AreEqual(std::string("pattern"), st.nextToken());
		Assert::AreEqual(std::string("("), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());
		Assert::AreEqual(std::string("x"), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());
		Assert::AreEqual(std::string(","), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());
		Assert::AreEqual(std::string("x"), st.nextToken());
		Assert::AreEqual(std::string("+"), st.nextToken());
		Assert::AreEqual(std::string("y"), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());
		Assert::AreEqual(std::string(")"), st.nextToken());

		st = StringTokenizer("pattern(\"x\",_)", QUERY_PREPROCESSOR);
		Assert::AreEqual(std::string("pattern"), st.nextToken());
		Assert::AreEqual(std::string("("), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());
		Assert::AreEqual(std::string("x"), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());
		Assert::AreEqual(std::string(","), st.nextToken());
		Assert::AreEqual(std::string("_"), st.nextToken());
		Assert::AreEqual(std::string(")"), st.nextToken());

		st = StringTokenizer("p.procName=\"First\"", QUERY_PREPROCESSOR);
		Assert::AreEqual(std::string("p"), st.nextToken());
		Assert::AreEqual(std::string("."), st.nextToken());
		Assert::AreEqual(std::string("procName"), st.nextToken());
		Assert::AreEqual(std::string("="), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());
		Assert::AreEqual(std::string("First"), st.nextToken());
		Assert::AreEqual(std::string("\""), st.nextToken());

	}

	TEST_METHOD(TestQueryValidator_Variable_Only) {
		QueryValidator *validator = QueryValidator::getInstance();
		// success
		validator->initStringTokenizer("\"a\"");
		validator->getNextToken();
		Assert::IsTrue(validator->isVariable("\""));

		// failure
		validator->initStringTokenizer("\"a"); // no back "
		validator->getNextToken();
		Assert::IsFalse(validator->isVariable("\""));

		validator->initStringTokenizer("a\""); // no front "
		validator->getNextToken();
		Assert::IsFalse(validator->isVariable(""));

		validator->initStringTokenizer("a"); // no front and back "
		validator->getNextToken();
		Assert::IsFalse(validator->isVariable(""));

		validator->initStringTokenizer("\"\""); // no name inside
		validator->getNextToken();
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
		Assert::IsTrue(validator->isRelationshipArgument("(", validator->getRelationshipTable()->find(RelationshipType::PARENT))); // Parent
		
		validator->initStringTokenizer("(p,p)");
		Assert::IsTrue(validator->isRelationshipArgument("(", validator->getRelationshipTable()->find(RelationshipType::CALLS))); // Calls

		validator->initStringTokenizer("(p,var1)");
		Assert::IsTrue(validator->isRelationshipArgument("(", validator->getRelationshipTable()->find(RelationshipType::MODIFIES))); // Modifies


		// failure
		validator->initStringTokenizer("(1,)"); // no second arg, with comma
		//Assert::IsFalse(validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)));
		auto funcPtr = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<std::runtime_error>(funcPtr);

		validator->initStringTokenizer("(1)"); // only 1 arg, no comma
		auto funcPtr2 = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<std::runtime_error>(funcPtr2);

		validator->initStringTokenizer("(,2)"); // no first arg, with comma
		auto funcPtr3 = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<std::runtime_error>(funcPtr3);

		validator->initStringTokenizer("p,p)"); // parent should have stmt args, but both args are procedure
		auto funcPtr4 = [validator] { validator->isRelationshipArgument("p", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<std::runtime_error>(funcPtr4);

		validator->initStringTokenizer("p,1)"); // parent should have stmt args, but both args are procedure
		auto funcPtr5 = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<std::runtime_error>(funcPtr5);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Parent) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		validator->clearSynonymOccurence();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;\nSelect p")); //
		//Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(1,2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(s1,s2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(1,s2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(s1,2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(ifstmt1, ifstmt2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(ifstmt1, 1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(w1, w2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(w1, 1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(s1,_)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(_,s1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(_,_)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		// failure
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parents(bb,cc)");
		validator->getNextToken();
		auto funcPtr = [validator] { validator->isRelationship("Parents"); };
		Assert::ExpectException<std::runtime_error>(funcPtr);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(p,q)"); // parent should have stmt args, but both args are procedure
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr2 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr2);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(s1,q)"); // parent
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr3 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr3);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(p,s2)"); // parent
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr4 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr4);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(s1)"); // parent
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr5 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr5);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(s1,)"); // parent
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr6 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr6);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(_,)"); // parent
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr7 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr7);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(\"x\",\"y\")"); // parent
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr8 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr8);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Parent(\"x\",_)"); // parent
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Parent"));
		auto funcPtr9 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<std::runtime_error>(funcPtr9);


	}

	TEST_METHOD(TestQueryValidator_Relationship_Follows) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();
	
		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Follows(s1,2)"); // follows
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Follows(c1,c2)"); // calls
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Follows(ifstmt1,c2)"); // if & call
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));


		// failure
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Follows(p,q)"); // follows
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Follows"));
		auto funcPtrError1 = [validator] { validator->isRelationship("Follows"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError1);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Modifies) {

		// modifies and uses should not have _ as first argument

		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
	
		// success
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Modifies(p,var1)"); // proc & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));
		
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Modifies(s1,var1)"); // stmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Modifies(ifstmt1,var1)"); // ifstmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Modifies(w1,var1)"); // while & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Modifies(c1,var1)"); // call & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		// failure
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Modifies(_,var1)"); // _ & var
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Modifies"));
		auto funcPtrError1 = [validator] { validator->isRelationship("Modifies"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError1);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Uses) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Uses(p,var1)"); // proc & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Uses(s1,var1)"); // stmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Uses(ifstmt1,var1)"); // ifstmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Uses(w1,var1)"); // while & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Uses(c1,var1)"); // call & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		// failure
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Uses(_,var1)"); // _ & var
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Uses"));
		auto funcPtrError1 = [validator] { validator->isRelationship("Uses"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError1);


		validator->clearSynonymOccurence();
		validator->initStringTokenizer("Uses(c1,3)"); // call & constant
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Uses"));
		auto funcPtrError2 = [validator] { validator->isRelationship("Uses"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError2);

	}

	TEST_METHOD(TestQueryValidator_Clause_Result) {
		QueryValidator *validator = QueryValidator::getInstance();

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->initStringTokenizer("p"); // declared synonym
		Assert::IsTrue(validator->isClauseResult(validator->getNextToken()));
		
		validator->initStringTokenizer("BOOLEAN"); // boolean!
		Assert::IsTrue(validator->isClauseResult(validator->getNextToken()));

		validator->initStringTokenizer("z"); // not declared synonym
		Assert::IsFalse(validator->isClauseResult(validator->getNextToken()));

		validator->initStringTokenizer(""); // empty
		Assert::IsFalse(validator->isClauseResult(validator->getNextToken()));

	}

	TEST_METHOD(TestQueryValidator_Clause_Pattern) {
		QueryValidator *validator = QueryValidator::getInstance();

		//Assert::IsTrue(validator->isWildcard("_"));

		// populate the synonym table first
		validator->clearSynonymOccurence();
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1, a2;if ifstmt;while w;variable v;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(v,\"x+y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(\"x\",\"x+y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(\"x\", _\"y\"_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,\"x\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,_\"x\"_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(\"x\",_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,_   \"x\"_)"); // spaces
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("w(_,_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("w"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("w(\"x\",_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("w"));
		
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("ifstmt(\"x\",_,_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("ifstmt(\"x\",_,\"x\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("ifstmt(\"x\",\"x\",_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("ifstmt(\"x\",\"x\",\"y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));
		
		// Failure
		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(a1,_)");			// left = a1 (synonym)
		validator->getNextToken();
		auto funcPtrError1 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError1);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(a2,_)");			// left = a2 (synonym)
		validator->getNextToken();
		//Assert::IsFalse(validator->isClausePattern("a1"));
		auto funcPtrError2 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError2);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(\"x + 1\",_)"); // first arg is expression
		validator->getNextToken();
		//Assert::IsFalse(validator->isClausePattern("a1"));
		auto funcPtrError3 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError3);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(x,_)");
		validator->getNextToken();
		//Assert::IsFalse(validator->isClausePattern("a1"));
		auto funcPtrError4 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError4);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,x)");
		validator->getNextToken();
		//Assert::IsFalse();
		auto funcPtrError5 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError5);


		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,_\"x+1\")");	// right wrong expressions
		validator->getNextToken();
		auto funcPtrError6 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError6);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(x,x)"); // left and right are not variables
		validator->getNextToken();
		auto funcPtrError7 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError7);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,a1)");			// right is not a variable
		validator->getNextToken();
		auto funcPtrError8 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError8);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("w(\"x\",\"x+y\")"); // second arg is not _
		validator->getNextToken();
		auto funcPtrError9 = [validator] { validator->isClausePattern("w"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError9);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("w(w,_)");		// no control variable, use synonym while
		validator->getNextToken();
		auto funcPtrError10 = [validator] { validator->isClausePattern("w"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError10);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("ifstmt(a,_,_)");	// no control variable, use synonym assign
		validator->getNextToken();
		auto funcPtrError11 = [validator] { validator->isClausePattern("ifstmt"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError11);
		
	}

	TEST_METHOD(TestQueryValidator_Pattern_Expr_Only) {
		QueryValidator *validator = QueryValidator::getInstance();
		// verify expression

		validator->initStringTokenizer("x+1");
		Assert::IsTrue(validator->isExpression("x"));

		validator->initStringTokenizer("x+1+z");
		Assert::IsTrue(validator->isExpression("x"));

		validator->initStringTokenizer("+1");
		Assert::IsFalse(validator->isExpression("+"));


		// success
		validator->initStringTokenizer("\"x\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isPatternExprArgument("\""));

		validator->initStringTokenizer("\"x+1\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isPatternExprArgument("\""));

		validator->initStringTokenizer("_\"x\"_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isPatternExprArgument("_"));


		// failure
		validator->initStringTokenizer("_)");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("_"));
		auto funcPtrError1 = [validator] { validator->isPatternExprArgument("_"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError1);

		validator->initStringTokenizer("\"x+1)");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("\""));
		auto funcPtrError2 = [validator] { validator->isPatternExprArgument("\""); };
		Assert::ExpectException<std::runtime_error>(funcPtrError2);



		validator->initStringTokenizer("_\"x+1\")");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("_"));
		auto funcPtrError3 = [validator] { validator->isPatternExprArgument("_"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError3);


		validator->initStringTokenizer("_\"x+1\")");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("_"));
		auto funcPtrError4 = [validator] { validator->isPatternExprArgument("_"); };
		Assert::ExpectException<std::runtime_error>(funcPtrError4);


		validator->initStringTokenizer("\"x+1\"_)");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("\""));
		auto funcPtrError5 = [validator] { validator->isPatternExprArgument("\""); };
		Assert::ExpectException<std::runtime_error>(funcPtrError5);



		validator->initStringTokenizer("\"+1\")");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("\""));
		auto funcPtrError6 = [validator] { validator->isPatternExprArgument("\""); };
		Assert::ExpectException<std::runtime_error>(funcPtrError6);


	}

	TEST_METHOD(TestQueryValidator_Clause_With) {
		QueryValidator *validator = QueryValidator::getInstance();

		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1, a2;if ifstmt;while w;variable v;call c;prog_line pl1, pl2;constant const;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->initStringTokenizer("p.procName=\"First\"");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("p"));


		validator->initStringTokenizer("c.procName=\"First\"");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("c"));
		
		validator->initStringTokenizer("c.stmt#=10");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("c"));

		validator->initStringTokenizer("const.value=1");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("const"));

		validator->initStringTokenizer("pl1=2");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("pl1"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		validator->initStringTokenizer("c.stmt# = const.value");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("c"));
		

		validator->initStringTokenizer("const.value = c.stmt#");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("const"));

		validator->initStringTokenizer("c.stmt# = ifstmt.stmt#");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("c"));

		validator->initStringTokenizer("c.procName = p.procName");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("c"));
		
		validator->initStringTokenizer("c.procName = v.varName");
		validator->getNextToken();
		Assert::IsTrue(validator->isClauseWith("c"));

		
		// Failure
		validator->initStringTokenizer("2 = pl1");
		validator->getNextToken();
		auto error1 = [validator] { validator->isClauseWith("2"); }; // invalid format
		Assert::ExpectException<std::runtime_error>(error1);

		validator->initStringTokenizer("c.stmt# = \"hello\"");
		validator->getNextToken();
		auto error2 = [validator] { validator->isClauseWith("c"); }; // incorrect value type
		Assert::ExpectException<std::runtime_error>(error2);

		validator->initStringTokenizer("p.procName = 1");
		validator->getNextToken();
		auto error3 = [validator] { validator->isClauseWith("p"); }; // incorrect value type
		Assert::ExpectException<std::runtime_error>(error3);

		validator->initStringTokenizer("p.varName = \"hello\"");
		validator->getNextToken();
		auto error4 = [validator] { validator->isClauseWith("p"); }; // incorrect attribute name
		Assert::ExpectException<std::runtime_error>(error4);

		validator->initStringTokenizer("const.procName = \"hello\"");
		validator->getNextToken();
		auto error5 = [validator] { validator->isClauseWith("const"); }; // incorrect attribute name
		Assert::ExpectException<std::runtime_error>(error5);

		validator->initStringTokenizer("c.stmt# = def.value");
		validator->getNextToken();
		auto error100 = [validator] { validator->isClauseWith("c"); }; // not declared synonym
		Assert::ExpectException<std::runtime_error>(error100);

		validator->initStringTokenizer("c.stmt# = p.procName");
		validator->getNextToken();
		auto error101 = [validator] { validator->isClauseWith("c"); }; // incorrect type
		Assert::ExpectException<std::runtime_error>(error101);

		validator->initStringTokenizer("p.procName = const.value");
		validator->getNextToken();
		auto error102 = [validator] { validator->isClauseWith("p"); }; // incorrect type
		Assert::ExpectException<std::runtime_error>(error102);

	}

	};
}