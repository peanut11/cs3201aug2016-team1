#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryValidator.h"
#include "../SPA/QueryProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTesting {
	TEST_CLASS(TestQueryValidator) {
public:
	
	TEST_METHOD(TestQueryValidator_Full_Query) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// Success test cases
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect p such that Parent(s1,s2)"));
		Assert::AreEqual(6, validator->getSynonymTable()->size());
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect p such that Parent(1,2)"));
		Assert::AreEqual(6, validator->getSynonymTable()->size());
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect p such that Parent(1,_)"));
		Assert::AreEqual(6, validator->getSynonymTable()->size());
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect p such that Parent(_,2)"));
		Assert::AreEqual(6, validator->getSynonymTable()->size());
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect p such that Parent(_,_)"));
		Assert::AreEqual(6, validator->getSynonymTable()->size());
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect p such that Parent(_,_) pattern a1(\"x\",\"y\")"));
		Assert::AreEqual(6, validator->getSynonymTable()->size());
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success follows
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Follows (s1, 3)"));
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Follows (s1, 3) pattern a1(\"x\",\"y\")"));
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Follows (s1, 3) pattern a1(\"x\",\"y\")"));
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// Failed test cases
		//Assert::IsFalse(validator->isValidQuery("procedure p;assign a;\nSelect"));
		//validator->clearSynonymTable();

		// if ifstmt;whilew;\nSelect w such that Parent(ifstmt, w)

	}


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

	TEST_METHOD(TestQueryValidator_Relationship_Parent) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->initStringTokenizer("Parent(1,2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,s2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(1,s2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(ifstmt1, ifstmt2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(ifstmt1, 1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(w1, w2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(w1, 1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,_)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(_,s1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(_,_)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		// failure
		validator->initStringTokenizer("Parent(p,q)"); // parent should have stmt args, but both args are procedure
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,q)"); // parent
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(p,s2)"); // parent
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1)"); // parent
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(s1,)"); // parent
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(_,)"); // parent
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(\"x\",\"y\")"); // parent
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent(\"x\",_)"); // parent
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Parent"));



	}

	TEST_METHOD(TestQueryValidator_Relationship_Follows) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();
	
		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->initStringTokenizer("Follows(s1,2)"); // follows
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows(c1,c2)"); // calls
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows(ifstmt1,c2)"); // if & call
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));


		// failure
		validator->initStringTokenizer("Follows(p,q)"); // follows
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Follows"));


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
		validator->initStringTokenizer("Modifies(p,var1)"); // proc & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->initStringTokenizer("Modifies(s1,var1)"); // stmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->initStringTokenizer("Modifies(ifstmt1,var1)"); // ifstmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->initStringTokenizer("Modifies(w1,var1)"); // while & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->initStringTokenizer("Modifies(c1,var1)"); // call & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		// failure
		validator->initStringTokenizer("Modifies(_,var1)"); // _ & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

	}

	TEST_METHOD(TestQueryValidator_Relationship_Uses) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->initStringTokenizer("Uses(p,var1)"); // proc & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->initStringTokenizer("Uses(s1,var1)"); // stmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->initStringTokenizer("Uses(ifstmt1,var1)"); // ifstmt & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->initStringTokenizer("Uses(w1,var1)"); // while & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->initStringTokenizer("Uses(c1,var1)"); // call & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		// failure
		validator->initStringTokenizer("Uses(_,var1)"); // _ & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

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
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->initStringTokenizer("a1(\"x\",\"x+y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(\"x\", _\"y\"_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(_,\"x+1\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(_,_\"x+1\"_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(\"x\",_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(a1,_)");			// left = a1 (synonym)
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(_,_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(_,_   \"x+1\"_)"); // spaces
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("w(_,_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("w"));

		validator->initStringTokenizer("w(\"x\",_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("w"));

		validator->initStringTokenizer("ifstmt(\"x\",_,_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));

		validator->initStringTokenizer("ifstmt(\"x\",_,\"x+y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));

		validator->initStringTokenizer("ifstmt(\"x\",\"x+1\",_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));

		validator->initStringTokenizer("ifstmt(\"x\",\"x+1\",\"x+y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("ifstmt"));
		
		// Failure
		validator->initStringTokenizer("a1(\"x + 1\",_)");
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(x,_)");
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(_,x)");
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(_,_\"x+1\")");
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(x,x)"); // left and right are variables
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(_,a1)");			// right = a1 (synonym)
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("a1"));

		validator->initStringTokenizer("w(\"x\",\"x+y\")");
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("w"));

		validator->initStringTokenizer("w(w,_)");
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("w"));

		validator->initStringTokenizer("ifstmt(a,_,_)");
		validator->getNextToken();
		Assert::IsFalse(validator->isClausePattern("ifstmt"));
		
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
		validator->initStringTokenizer("\"x+1\",");
		validator->getNextToken();
		Assert::IsTrue(validator->isPatternExprArgument("\""));

		validator->initStringTokenizer("_\"x+1\"_,");
		validator->getNextToken();
		Assert::IsTrue(validator->isPatternExprArgument("_"));

		// failure
		validator->initStringTokenizer("_,");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument("_"));

		validator->initStringTokenizer("\"x+1\"");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument("\""));

		validator->initStringTokenizer("_\"x+1\"_");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument("_"));

		validator->initStringTokenizer("\"x+1,");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument("\""));

		validator->initStringTokenizer("_\"x+1\",");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument("_"));

		validator->initStringTokenizer("\"x+1\"_,");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument("\""));

		validator->initStringTokenizer("\"+1\",");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument("\""));

		validator->initStringTokenizer(",");
		validator->getNextToken();
		Assert::IsFalse(validator->isPatternExprArgument(","));



	}

	};
}