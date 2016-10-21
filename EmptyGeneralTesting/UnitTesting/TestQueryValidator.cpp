#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryValidator.h"
#include "../SPA/QueryProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestQueryValidator) {
public:
	
	TEST_METHOD(TestQueryValidator__Iteration_3_Full_Query) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		std::string declaration = "procedure p, q;variable var1;assign a1, a2;if ifstmt;while w;stmt s1, s2, s3, s4, s5;progline n1, n2;call c;constant const;\n";


		Assert::IsTrue(validator->isValidQuery(declaration + "Select p.procName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p.procName such that Parent(s1,_) Next(s1, s2)"));
		//Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		

		Assert::IsTrue(validator->isValidQuery(declaration + "Select c.stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select var1.varName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select ifstmt.stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select w.stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1.stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1.stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select const.value such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select n1 such that Parent(s1,_) and Next(s1, s2)"));

		Assert::IsTrue(validator->isValidQuery(declaration + "Select p.      procName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select c   .  stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select var1 . varName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select ifstmt     .stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select w.stmt # such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1.  stmt # such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1  .  stmt  # such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select const  .   value such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select n1 such that Parent(s1,_) and Next(s1, s2)"));


		// Success Tuple with synonyms only
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p, q> such that Parent(s1,_) and Next(s1, s2)"));

		// Success Tuple with single synonym with or without attrName
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p.procName> such that Parent(s1,_) and Next(s1, s2)"));

		
		// Success Tuple with mutliple synonym and attrName
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p.procName, q> such that Parent(s1,_) and Next(s1, s2)"));

		// Success Tuple with similar synonyms and attrNames
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p, p> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p.procName, p> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p, p.procName> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p, q.procName> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p, c.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p, c.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <p.procName, w.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <var1.varName, w.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <const.value, w.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <w.stmt#, var1.varName> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <a1.stmt#, const.value> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <n1, const.value> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <a1.stmt#, n2> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <n1, n2> such that Parent(s1,_) and Next(s1, s2)"));
		
		// Success Tuple with two or more synonyms
		Assert::IsTrue(validator->isValidQuery(declaration + "Select <n1, p, var1, const, w, n2, q, const> such that Parent(s1,_) and Next(s1, s2)"));

		
		// Failure, incorrect attrRef syntax
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p.proc such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select a1.stmt such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select const.val such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p.value such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select c.stm such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select c.# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p.value such that Parent(s1,_) and Next(s1, s2)"));

		// Failure, missing attrRef syntax
		Assert::IsFalse(validator->isValidQuery(declaration + "Select .procName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p. such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select c # such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select var1 varName such that Parent(s1,_) and Next(s1, s2)"));



		// Failure, incorrect attrRef combination
		Assert::IsFalse(validator->isValidQuery(declaration + "Select var1.stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p.varName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p.stmt# such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select a1.procName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select w.procName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select ifstmt.procName such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p.value such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select n1.stmt# such that Parent(s1,_) and Next(s1, s2)"));



		// Failure, Tuple with wrong format
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <<p, q> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <p, q>> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <<p, q>> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p, q> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <p, q such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select p, q such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <p q> such that Parent(s1,_) and Next(s1, s2)"));

		// Failure, Tuple correct format, wrong element
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <z, q> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <p, z> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <z, abc> such that Parent(s1,_) and Next(s1, s2)"));


		// Failure, Tuple with wrong synonym attrName
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <p.value, q> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <p, q.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <p.value, q.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <c.value, q.varName> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <var1.procName, q.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <n1.stmt#, n2> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <n1, n2.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		Assert::IsFalse(validator->isValidQuery(declaration + "Select <n1.stmt#, n2.stmt#> such that Parent(s1,_) and Next(s1, s2)"));
		
		
	}

	TEST_METHOD(TestQueryValidator__Iteration_2_Full_Query) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		std::string declaration = "stmtLst sl1; procedure p, q;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;call c;constant const;\n";
		
	
		// Follows (s, s), Parent*(_, _) must return error, cos same synonym
		// output is "wrong". while w; variable v; Select v such that Uses (w, _)
		// output is "wrong". Select v such that Modifies (a1, "iter") pattern a("left", _)
		// crash. assign a; Select a such that Parent*(38, a)
		
		// Success Result clause is statement list
		Assert::IsTrue(validator->isValidQuery(declaration + "Select sl1 such that Modifies(a1, \"x\") pattern a1(\"x\",\"y+1\")"));
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Success Modifies procedure
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Modifies(a1, \"x\") pattern a1(\"x\",\"y+1\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Modifies(\"First\", \"x\") pattern a1(\"x\",\"y+1\")"));
		
		// Success Uses procedure
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(a1, \"x\") pattern a1(\"x\",\"y+1\")"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(\"First\", \"x\") pattern a1(\"x\",\"y+1\")"));
		
		// Success Parent*
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(s1,s2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent*(3,s1)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(_,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(_,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(s1,s2) pattern a1(\"x\",_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(s1,s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(s1,_) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(_,s1) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(_,_) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(_,_) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(_,_) pattern a1(_,_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent*(s1, s2) pattern a1(\"x\",\"y\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent*(_,_) pattern a1(\"x\", \"y+1\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent*(s1, s2) Parent*(_, _) pattern a1(\"x\",_\"y+1\"_)"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Success Follows*
		Assert::IsTrue(validator->isValidQuery(declaration + "Select ifstmt such that Follows* (5, ifstmt)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows* (s1, 3)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows* (s1, 3) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(s1, 2) Parent*(s1, s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(s1, _) Parent*(s1, s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(_, _) Parent*(_, s2) pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(_, _) Parent*(_, s2) pattern a1(_,_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(_, _) Parent*(_, s2) pattern a1(_,_\"y+1\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(_, _) Parent*(_, s2) pattern a1(_,_\"1+y+x\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Follows*(_, _) Parent*(_, s2) pattern a1(_,\"1+y+x\")"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Success Next
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(_,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(_,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,n1)")); // same synonym in Next
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(a1,w)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(ifstmt,s1)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(w,s1)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(w,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(w,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next(n1,n1)"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Success Next*
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(n1,2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(n1,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(_,n2)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(n1,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(_,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(n1,n1)")); // same synonym in Next
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(a1,w)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(ifstmt,s1)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(w,s1)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(w,_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Next*(w,n2)"));

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

		// Success Calls
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls(p,q)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls(\"First\",q)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls(\"First\",\"Second\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls(\"First\",_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls(p,\"Second\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls(_,\"Second\")"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Success Calls*
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls*(p,q)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls*(\"First\",q)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls*(\"First\",\"Second\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls*(\"First\",_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls*(p,\"Second\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) and Calls*(_,\"Second\")"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Success With
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with s1.stmt# = 1")); // 1 with clause
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = const.value")); // 1 with clause
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = c.stmt#"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = s1.stmt#"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = a1.stmt#"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = ifstmt.stmt#"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = w.stmt#"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = n2"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) pattern a1(\"x\",_\"y+1\"_) with s1.stmt# = 1 s2.stmt# = 4")); // 2 with clauses
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with s1.stmt# = 1 such that Follows(s1, _)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with a1.stmt# = 1 such that Follows(s1, _)"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Success Pattern, allows minus and multiply
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) pattern a1(\"x\", \"x-y\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) pattern a1(\"x\", \"x*y\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) pattern a1(\"x\", \"x+y-z\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) pattern a1(\"x\", \"w*x+y-z\")"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// Failure
		/*
		auto error1 = [validator] { 
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Next(s1,_)");
		};
		Assert::ExpectException<Exceptions>(error1);
		
		auto error2 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Next(_,s2)");
		};
		Assert::ExpectException<Exceptions>(error2);

		auto error3 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Next(p,s2)");
		};
		Assert::ExpectException<Exceptions>(error3);
		*/
		auto error4 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Affects(s1,_)");
		};
		Assert::ExpectException<Exceptions>(error4);

		auto error5 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Affects(_,s2)");
		};
		Assert::ExpectException<Exceptions>(error5);

		auto error6 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s2) Affects(s1,p)");
		};
		Assert::ExpectException<Exceptions>(error6);

		// Failure, same synonym in relationships
		auto error7 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Parent(s1, s1)");
		};
		Assert::ExpectException<Exceptions>(error7);

		auto error8 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Affects(a1, a1)");
		};
		Assert::ExpectException<Exceptions>(error8);

		auto error9 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Affects*(a1, a1)");
		};
		Assert::ExpectException<Exceptions>(error9);

		// Failure, progline
		auto error10 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select p such that Parent(s1,s2) with n1 = p.procName");
		};
		Assert::ExpectException<Exceptions>(error10);


		// Failure, incorrect number of arguments
		auto error100 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Affects*(a1, a2, _)");
		};
		Assert::ExpectException<Exceptions>(error100);

		auto error101 = [validator] {
			std::string declaration = "procedure p;assign a1, a2;if ifstmt;while w;stmt s1, s2;progline n1, n2;\n";
			validator->isValidQuery(declaration + "Select s1 such that Affects*(a1)");
		};
		Assert::ExpectException<Exceptions>(error101);


		

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
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Uses(5, \"x\")"));
		
		// 1 common synonym between clauses
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(a1, \"x\") pattern a1(\"x\",_\"y\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(a1, \"x\") pattern a1(\"x\",_\"y+1\"_)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select a1 such that Uses(a1, \"x\") pattern a1(\"x\",\"y+1\")"));
		Assert::IsTrue(validator->isValidQuery("assign a, a1; variable v;\nSelect a such that Uses(a,v) pattern a1(v,_)"));
		
		//Logger::WriteMessage(validator->getSynonymOccurence()->toString().c_str());
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());
		


		// success MODIFIES
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Modifies(a1, \"x\")"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select s1 such that Modifies(10, \"x\")"));

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
		Assert::ExpectException<Exceptions>(funcPtrExprError1);

		auto funcPtrExprError2 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",_\"+1\")"); };
		Assert::ExpectException<Exceptions>(funcPtrExprError2);

		auto funcPtrExprError3 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",\"+1\"_)"); };
		Assert::ExpectException<Exceptions>(funcPtrExprError3);

		auto funcPtrExprError4 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",\"_\")"); };
		Assert::ExpectException<Exceptions>(funcPtrExprError4);

		auto funcPtrExprError5 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) pattern a1(\"x\",y)"); };
		Assert::ExpectException<Exceptions>(funcPtrExprError5);

		auto funcPtrExprError6 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(0,s1)"); };
		Assert::ExpectException<Exceptions>(funcPtrExprError6);

		auto funcPtrExprError7 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1,0)"); };
		Assert::ExpectException<Exceptions>(funcPtrExprError7);


		/*
		// more than 1 common synonym between clauses
		auto funcPtrError1 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Follows(s1, s2) Parent(s1, s2) pattern a1(\"x\",\"y\")"); };
		Assert::ExpectException<Exceptions>(funcPtrError1);
		
		auto funcPtrError2 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;variable v\nSelect a such that Uses (a1, v) pattern a1(v, _)"); };
		Assert::ExpectException<Exceptions>(funcPtrError2);
		*/


		// USES and MODIFIES cannot have wildcard as first argument
		auto funcPtrWildcardError1 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) Modifies(_, \"x\") pattern a1(\"x\",\"y\")"); };
		Assert::ExpectException<Exceptions>(funcPtrWildcardError1);

		auto funcPtrWildcardError2 = [validator] { validator->isValidQuery("procedure p;assign a1;if ifstmt;while w;stmt s1, s2;\nSelect s1 such that Parent(s1, s2) Uses(_, \"x\") pattern a1(\"x\",\"y\")"); };
		Assert::ExpectException<Exceptions>(funcPtrWildcardError2);

		// Failure, wrong declaration
		Assert::IsFalse(validator->isValidQuery("proc p;\nSelect p")); // wrong syntax
		Assert::IsFalse(validator->isValidQuery("Procedure p;\nSelect p")); // wrong case
		Assert::IsFalse(validator->isValidQuery("StmtLst sl1;\nSelect sl1")); // wrong case
		Assert::IsFalse(validator->isValidQuery("Stmtlst sl1;\nSelect sl1")); // wrong case 

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
		Assert::ExpectException<Exceptions>(funcPtr); // try insert two procedure p, return exception
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
		Assert::IsTrue(validator->isValidQuery("procedure p, q;variable var1;\nSelect p"));
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());


		// success
		validator->initStringTokenizer("(1,2)"); // stmt numbers
		Assert::IsTrue(validator->isRelationshipArgument("(", validator->getRelationshipTable()->find(RelationshipType::PARENT))); // Parent
		
		validator->initStringTokenizer("(p,q)");
		Assert::IsTrue(validator->isRelationshipArgument("(", validator->getRelationshipTable()->find(RelationshipType::CALLS))); // Calls

		validator->initStringTokenizer("(p,var1)");
		Assert::IsTrue(validator->isRelationshipArgument("(", validator->getRelationshipTable()->find(RelationshipType::MODIFIES))); // Modifies


		// failure
		validator->initStringTokenizer("(1,)"); // no second arg, with comma
		//Assert::IsFalse(validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)));
		auto funcPtr = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<Exceptions>(funcPtr);

		validator->initStringTokenizer("(1)"); // only 1 arg, no comma
		auto funcPtr2 = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<Exceptions>(funcPtr2);

		validator->initStringTokenizer("(,2)"); // no first arg, with comma
		auto funcPtr3 = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<Exceptions>(funcPtr3);

		validator->initStringTokenizer("p,p)"); // parent should have stmt args, but both args are procedure
		auto funcPtr4 = [validator] { validator->isRelationshipArgument("p", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<Exceptions>(funcPtr4);

		validator->initStringTokenizer("p,1)"); // parent should have stmt args, but both args are procedure
		auto funcPtr5 = [validator] { validator->isRelationshipArgument("(", validator->getRelationshipTable()->getObject(4)); };
		Assert::ExpectException<Exceptions>(funcPtr5);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Parent) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		validator->clearSynonymOccurence();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;\nSelect p")); //
		//Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success Parent
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

		// Success Parent*
		validator->initStringTokenizer("Parent*(1,2)"); 
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(s1,s2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(1,s2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(s1,2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(ifstmt1, ifstmt2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(ifstmt1, 1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(w1, w2)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(w1, 1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(s1,_)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(_,s1)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		validator->initStringTokenizer("Parent*(_,_)"); // parent
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Parent"));

		// failure
		validator->initStringTokenizer("Parents(bb,cc)");
		validator->getNextToken();
		auto funcPtr = [validator] { validator->isRelationship("Parents"); };
		Assert::ExpectException<Exceptions>(funcPtr);

		validator->initStringTokenizer("Parent(p,q)"); // parent should have stmt args, but both args are procedure
		validator->getNextToken();
		auto funcPtr2 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr2);

		validator->initStringTokenizer("Parent(s1,q)"); // 2nd arugment is procedure
		validator->getNextToken();
		auto funcPtr3 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr3);

		validator->initStringTokenizer("Parent(p,s2)"); // parent
		validator->getNextToken();
		auto funcPtr4 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr4);

		validator->initStringTokenizer("Parent(s1)"); // missing 2nd argument
		validator->getNextToken();
		auto funcPtr5 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr5);

		validator->initStringTokenizer("Parent(s1,)"); // missing 2nd argument
		validator->getNextToken();
		auto funcPtr6 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr6);

		validator->initStringTokenizer("Parent(_,)"); // parent
		validator->getNextToken();
		auto funcPtr7 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr7);

		validator->initStringTokenizer("Parent(\"x\",\"y\")"); // 
		validator->getNextToken();
		auto funcPtr8 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr8);

		validator->initStringTokenizer("Parent(\"x\",_)"); // 
		validator->getNextToken();
		auto funcPtr9 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr9);

		validator->initStringTokenizer("Parent(s1,s1)"); // same synonyms in both arguments
		validator->getNextToken();
		auto funcPtr10 = [validator] { validator->isRelationship("Parent"); };
		Assert::ExpectException<Exceptions>(funcPtr10);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Follows) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();
	
		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// Success Follows
		validator->initStringTokenizer("Follows(s1,2)"); // follows
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows(s1,s2)"); // follows
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows(c1,c2)"); // calls
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows(ifstmt1,c2)"); // if & call
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		// Success Follows*
		validator->initStringTokenizer("Follows*(s1,2)"); // follows
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows*(s1,s2)"); // follows
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows*(c1,c2)"); // calls
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		validator->initStringTokenizer("Follows*(ifstmt1,c2)"); // if & call
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Follows"));

		// failure
		validator->initStringTokenizer("Follows(p,q)"); // follows
		validator->getNextToken();
		auto funcPtrError1 = [validator] { validator->isRelationship("Follows"); };
		Assert::ExpectException<Exceptions>(funcPtrError1);

		validator->initStringTokenizer("Follows(s1,s1)"); // same synonym for both args
		validator->getNextToken();
		auto funcPtrError2 = [validator] { validator->isRelationship("Follows"); };
		Assert::ExpectException<Exceptions>(funcPtrError2);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Modifies) {

		// modifies and uses should not have _ as first argument

		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		//Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
	
		// success
		validator->initStringTokenizer("Modifies(p,var1)"); // proc & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));
		
		validator->initStringTokenizer("Modifies(p,\"x\")"); // proc & var
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

		validator->initStringTokenizer("Modifies(\"First\",var1)"); // procedure name
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));
		
		validator->initStringTokenizer("Modifies(\"First\",\"x\")"); 
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		validator->initStringTokenizer("Modifies(1,var1)"); // constant & var
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Modifies"));

		Logger::WriteMessage(validator->getQueryTable().toString().c_str());


		// failure
		validator->initStringTokenizer("Modifies(_,var1)"); // _ & var
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Modifies"));
		auto funcPtrError1 = [validator] { validator->isRelationship("Modifies"); };
		Assert::ExpectException<Exceptions>(funcPtrError1);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Uses) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		//Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

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

		validator->initStringTokenizer("Uses(\"First\",var1)"); // procedure name
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		validator->initStringTokenizer("Uses(\"First\",\"x\")"); 
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));
	
		validator->initStringTokenizer("Uses(1,\"x\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Uses"));

		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// failure
		validator->initStringTokenizer("Uses(_,var1)"); // _ & var
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Uses"));
		auto funcPtrError1 = [validator] { validator->isRelationship("Uses"); };
		Assert::ExpectException<Exceptions>(funcPtrError1);

		validator->initStringTokenizer("Uses(c1,3)"); // call & constant
		validator->getNextToken();
		//Assert::IsFalse(validator->isRelationship("Uses"));
		auto funcPtrError2 = [validator] { validator->isRelationship("Uses"); };
		Assert::ExpectException<Exceptions>(funcPtrError2);

	}

	TEST_METHOD(TestQueryValidator_Relationship_Calls) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		// populate the synonym table first
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p,q;variable var1;stmt s1, s2;if ifstmt1, ifstmt2;while w1, w2;call c1, c2;\nSelect p")); //
		//Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->initStringTokenizer("Calls(p,q)");
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Calls"));

		validator->initStringTokenizer("Calls(\"First\",q)");
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Calls"));

		validator->initStringTokenizer("Calls(p,\"First\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Calls"));

		validator->initStringTokenizer("Calls*(p,\"First\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isRelationship("Calls"));
		//Logger::WriteMessage(validator->getQueryTable().toString().c_str());

		// failure
		validator->initStringTokenizer("Calls(1, q)"); // 
		validator->getNextToken();
		auto funcPtrError1 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError1);

		validator->initStringTokenizer("Calls(p, 5)"); // 
		validator->getNextToken();
		auto funcPtrError2 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError2);

		validator->initStringTokenizer("Calls(1, 5)"); // 
		validator->getNextToken();
		auto funcPtrError3 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError3);

		validator->initStringTokenizer("Calls*(1, q)"); // 
		validator->getNextToken();
		auto funcPtrError101 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError101);

		validator->initStringTokenizer("Calls*(p, 5)"); 
		validator->getNextToken();
		auto funcPtrError102 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError102);

		validator->initStringTokenizer("Calls*(1, 5)"); 
		validator->getNextToken();
		auto funcPtrError103 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError103);

		validator->initStringTokenizer("Calls*(1, )"); 
		validator->getNextToken();
		auto funcPtrError104 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError104);

		validator->initStringTokenizer("Calls*(,q)"); 
		validator->getNextToken();
		auto funcPtrError105 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError105);

		validator->initStringTokenizer("Calls*(,)");
		validator->getNextToken();
		auto funcPtrError106 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError106);

		validator->initStringTokenizer("Calls*()");
		validator->getNextToken();
		auto funcPtrError107 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError107);

		validator->initStringTokenizer("Calls*(,");
		validator->getNextToken();
		Assert::IsFalse(validator->isRelationship("Calls"));
		//auto funcPtrError108 = [validator] { validator->isRelationship("Calls"); };
		//Assert::ExpectException<Exceptions>(funcPtrError108);

		validator->initStringTokenizer("Calls*,)");
		validator->getNextToken();
		auto funcPtrError109 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError109);

		validator->initStringTokenizer("Calls*");
		validator->getNextToken();
		auto funcPtrError110 = [validator] { validator->isRelationship("Calls"); };
		Assert::ExpectException<Exceptions>(funcPtrError110);

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

		// populate the synonym table first
		validator->clearSynonymOccurence();
		validator->clearSynonymTable();
		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1, a2;if ifstmt;while w;variable v;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());

		// success
		validator->initStringTokenizer("a1(v,\"x+y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(\"x\",\"x+y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(\"x\",\"x-y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));
		
		validator->initStringTokenizer("a1(\"x\",\"x*y\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(\"x\",\"x+y-z\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		validator->initStringTokenizer("a1(\"x\",\"w*x+y-z\")");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

		
		validator->initStringTokenizer("a1(\"x\", _\"y\"_)");
		validator->getNextToken();
		Assert::IsTrue(validator->isClausePattern("a1"));

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
		Assert::ExpectException<Exceptions>(funcPtrError1);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(a2,_)");			// left = a2 (synonym)
		validator->getNextToken();
		//Assert::IsFalse(validator->isClausePattern("a1"));
		auto funcPtrError2 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<Exceptions>(funcPtrError2);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(\"x + 1\",_)"); // first arg is expression
		validator->getNextToken();
		//Assert::IsFalse(validator->isClausePattern("a1"));
		auto funcPtrError3 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<Exceptions>(funcPtrError3);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(x,_)");
		validator->getNextToken();
		//Assert::IsFalse(validator->isClausePattern("a1"));
		auto funcPtrError4 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<Exceptions>(funcPtrError4);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,x)");
		validator->getNextToken();
		//Assert::IsFalse();
		auto funcPtrError5 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<Exceptions>(funcPtrError5);


		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,_\"x+1\")");	// right wrong expressions
		validator->getNextToken();
		auto funcPtrError6 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<Exceptions>(funcPtrError6);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(x,x)"); // left and right are not variables
		validator->getNextToken();
		auto funcPtrError7 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<Exceptions>(funcPtrError7);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("a1(_,a1)");			// right is not a variable
		validator->getNextToken();
		auto funcPtrError8 = [validator] { validator->isClausePattern("a1"); };
		Assert::ExpectException<Exceptions>(funcPtrError8);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("w(\"x\",\"x+y\")"); // second arg is not _
		validator->getNextToken();
		auto funcPtrError9 = [validator] { validator->isClausePattern("w"); };
		Assert::ExpectException<Exceptions>(funcPtrError9);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("w(w,_)");		// no control variable, use synonym while
		validator->getNextToken();
		auto funcPtrError10 = [validator] { validator->isClausePattern("w"); };
		Assert::ExpectException<Exceptions>(funcPtrError10);

		validator->clearSynonymOccurence();
		validator->initStringTokenizer("ifstmt(a,_,_)");	// no control variable, use synonym assign
		validator->getNextToken();
		auto funcPtrError11 = [validator] { validator->isClausePattern("ifstmt"); };
		Assert::ExpectException<Exceptions>(funcPtrError11);
		
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
		Assert::ExpectException<Exceptions>(funcPtrError1);

		validator->initStringTokenizer("\"x+1)");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("\""));
		auto funcPtrError2 = [validator] { validator->isPatternExprArgument("\""); };
		Assert::ExpectException<Exceptions>(funcPtrError2);



		validator->initStringTokenizer("_\"x+1\")");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("_"));
		auto funcPtrError3 = [validator] { validator->isPatternExprArgument("_"); };
		Assert::ExpectException<Exceptions>(funcPtrError3);


		validator->initStringTokenizer("_\"x+1\")");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("_"));
		auto funcPtrError4 = [validator] { validator->isPatternExprArgument("_"); };
		Assert::ExpectException<Exceptions>(funcPtrError4);


		validator->initStringTokenizer("\"x+1\"_)");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("\""));
		auto funcPtrError5 = [validator] { validator->isPatternExprArgument("\""); };
		Assert::ExpectException<Exceptions>(funcPtrError5);



		validator->initStringTokenizer("\"+1\")");
		validator->getNextToken();
		//Assert::IsFalse(validator->isPatternExprArgument("\""));
		auto funcPtrError6 = [validator] { validator->isPatternExprArgument("\""); };
		Assert::ExpectException<Exceptions>(funcPtrError6);


	}

	TEST_METHOD(TestQueryValidator_Clause_With) {
		QueryValidator *validator = QueryValidator::getInstance();

		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1, a2;if ifstmt;while w;variable v;call c;progline pl1, pl2;constant const;\nSelect p")); //
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
		Assert::ExpectException<Exceptions>(error1);

		validator->initStringTokenizer("c.stmt# = \"hello\"");
		validator->getNextToken();
		auto error2 = [validator] { validator->isClauseWith("c"); }; // incorrect value type
		Assert::ExpectException<Exceptions>(error2);

		validator->initStringTokenizer("p.procName = 1");
		validator->getNextToken();
		auto error3 = [validator] { validator->isClauseWith("p"); }; // incorrect value type
		Assert::ExpectException<Exceptions>(error3);

		validator->initStringTokenizer("p.varName = \"hello\"");
		validator->getNextToken();
		auto error4 = [validator] { validator->isClauseWith("p"); }; // incorrect attribute name
		Assert::ExpectException<Exceptions>(error4);

		validator->initStringTokenizer("const.procName = \"hello\"");
		validator->getNextToken();
		auto error5 = [validator] { validator->isClauseWith("const"); }; // incorrect attribute name
		Assert::ExpectException<Exceptions>(error5);

		validator->initStringTokenizer("c.stmt# = def.value");
		validator->getNextToken();
		auto error100 = [validator] { validator->isClauseWith("c"); }; // not declared synonym
		Assert::ExpectException<Exceptions>(error100);

		validator->initStringTokenizer("c.stmt# = p.procName");
		validator->getNextToken();
		auto error101 = [validator] { validator->isClauseWith("c"); }; // incorrect type
		Assert::ExpectException<Exceptions>(error101);

		validator->initStringTokenizer("p.procName = const.value");
		validator->getNextToken();
		auto error102 = [validator] { validator->isClauseWith("p"); }; // incorrect type
		Assert::ExpectException<Exceptions>(error102);

	}

	TEST_METHOD(TestQueryValidator_Attribute_Ref) {
		QueryValidator *validator = QueryValidator::getInstance();

		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1, a2;if ifstmt;while w;variable v;call c;progline pl1, pl2;constant const;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());


		// success
		validator->initStringTokenizer("p.procName");
		validator->getNextToken();
		Assert::IsTrue(validator->isAttributeReference("p"));

		validator->initStringTokenizer("c.procName");
		validator->getNextToken();
		Assert::IsTrue(validator->isAttributeReference("c"));

		validator->initStringTokenizer("c.stmt#");
		validator->getNextToken();
		Assert::IsTrue(validator->isAttributeReference("c"));

		validator->initStringTokenizer("a1.stmt#");
		validator->getNextToken();
		Assert::IsTrue(validator->isAttributeReference("a1"));

		validator->initStringTokenizer("v.varName");
		validator->getNextToken();
		Assert::IsTrue(validator->isAttributeReference("v"));

		validator->initStringTokenizer("const.value");
		validator->getNextToken();
		Assert::IsTrue(validator->isAttributeReference("const"));


		// Failure
		validator->initStringTokenizer("p.varName");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("p"));

		validator->initStringTokenizer("p.stmt#");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("p"));

		validator->initStringTokenizer("p.value");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("p"));

		validator->initStringTokenizer("c.varName");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("c"));

		validator->initStringTokenizer("c.value");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("c"));

		validator->initStringTokenizer("const.varName");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("const"));

		validator->initStringTokenizer("const.procName");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("const"));

		validator->initStringTokenizer("const.stmt#");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("const"));

		// Failure, format error
		validator->initStringTokenizer("p.");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("p"));

		validator->initStringTokenizer("procName.p");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("procName"));

		// Failure, syntax error
		validator->initStringTokenizer("p.proc");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("p"));

		validator->initStringTokenizer("c.stmt");
		validator->getNextToken();
		Assert::IsFalse(validator->isAttributeReference("c"));

	}

	TEST_METHOD(TestQueryValidator_Tuple) {
		QueryValidator *validator = QueryValidator::getInstance();

		Assert::IsTrue(validator->isValidQuery("procedure p;assign a1, a2;if ifstmt;while w;variable v;call c;progline pl1, pl2;constant const;\nSelect p")); //
		Logger::WriteMessage(validator->getSynonymTable()->toString().c_str());
		
		// Success
		validator->initStringTokenizer("<p>");
		validator->getNextToken();
		Assert::IsTrue(validator->isTuple("<"));

		validator->initStringTokenizer("<p, a1, a2>");
		validator->getNextToken();
		Assert::IsTrue(validator->isTuple("<"));

		validator->initStringTokenizer("<p, a1, a2, w, v, c, pl1>");
		validator->getNextToken();
		Assert::IsTrue(validator->isTuple("<"));

		validator->initStringTokenizer("<p>>"); // althought its true, then next clause check will return false as the next token is ">"
		validator->getNextToken();
		Assert::IsTrue(validator->isTuple("<"));

		// Failure
		validator->initStringTokenizer("<p, >");
		validator->getNextToken();
		Assert::IsFalse(validator->isTuple("<"));

		validator->initStringTokenizer("<p such");
		validator->getNextToken();
		Assert::IsFalse(validator->isTuple("<"));
		
		validator->initStringTokenizer("<p");
		validator->getNextToken();
		Assert::IsFalse(validator->isTuple("<"));


		validator->initStringTokenizer("<<p>");
		validator->getNextToken();
		Assert::IsFalse(validator->isTuple("<"));



	}

	TEST_METHOD(TestQueryValidator_Query_Synonym_Group) {
		QueryProcessor *processor = QueryProcessor::getInstance();
		QueryValidator *validator = QueryValidator::getInstance();

		std::string declaration = "procedure p, q;variable v1, v2, v3;assign a1, a2, a3;if ifstmt;while w;stmt s1, s2, s3, s4, s5;progline n1, n2;call c;constant const;\n";

	
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p.procName such that Follows(s3,s1) Parent(s4,s5) Follows(s1,s4)"));
		Assert::IsTrue(validator->isValidQuery(declaration + "Select p.procName such that Follows(s3,s1) Parent(s4,s5) Follows(s4,s1)"));

		Assert::IsTrue(validator->isValidQuery(declaration + "Select <s1, s2, v2> such that Uses (s3, v1) and Modifies (s3, \"x\") and Follows (s1, s2) and Parent (s3, s1) and Uses (s2, v1) such that Uses(5, \"y\") and Follows(3, 4) pattern a1(v2, _\"x + y\"_) such that Affects(a1, a2) with a2.stmt#  = 20 such that Modifies(a3, v3) pattern a3(\"z\", _)"));

		Logger::WriteMessage(validator->getSynonymGroup()->toString().c_str());


	}

	TEST_METHOD(TestQueryValidator_Synonym_Group_Only) {
		QueryValidator *validator = QueryValidator::getInstance();

		SynonymGroup *mSynonymGroup = validator->getSynonymGroup();
		/*
		mSynonymGroup->insertSynonym("s3", 1);
		mSynonymGroup->insertSynonym("s1", 1);

		mSynonymGroup->insertSynonym("s4", 2);
		mSynonymGroup->insertSynonym("s5", 2);

		mSynonymGroup->insertSynonym("s5");
		mSynonymGroup->insertSynonym("s6", 2);

		mSynonymGroup->insertSynonym("s1");
		mSynonymGroup->insertSynonym("s4", 1);
		*/
		mSynonymGroup->insertSynonym("s3", 1);
		mSynonymGroup->insertSynonym("s1", 1);

		mSynonymGroup->insertSynonym("s4", 2);
		mSynonymGroup->insertSynonym("s5", 2);

		mSynonymGroup->insertSynonym("s6", 3);
		mSynonymGroup->insertSynonym("s7", 4);
		mSynonymGroup->insertSynonym("s8", 4);
		mSynonymGroup->insertSynonym("s9", 4);

		mSynonymGroup->insertSynonym("s10", 5);

		mSynonymGroup->insertSynonym("s7", 1);
		mSynonymGroup->insertSynonym("s3", 1);

		mSynonymGroup->insertSynonym("s10", 2);

		Logger::WriteMessage(mSynonymGroup->toString().c_str());

	}




	};
}