#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/Types.h"
#include <vector>
#include <string>
#include <iostream>
//#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestAssignPattern) {
public:
	TEST_METHOD(TestPatternShort) {
		PKB * pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = {"(", "3", "+", "x", "-", "y", "+", "gama", ")", "*", "y", "-", "alpha", "/", "beta" };
		InfixExpr infix = InfixExpr(arr, arr + sizeof(arr) / sizeof(arr[0]));
		pkb->putExprForStmt(10, pkb->infixToPostfix(infix));

		bool result = pkb->isAssignExactPattern(10, infix);

		Assert::IsTrue(result);
	}

	TEST_METHOD(TestPatternMedium) {
		PKB * pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "3", "+", "5", "*", "(", "4", "+", "2", "/", "3", "-", "1", ")", "+", "7" , "+", "9", "*", "8" };
		InfixExpr infix = InfixExpr(arr, arr + sizeof(arr) / sizeof(arr[0]));
		pkb->putExprForStmt(6, pkb->infixToPostfix(infix));

		StringToken p0[] = { "3", "5", "4", "2", "3", "/", "+", "1", "-", "*", "+", "7", "+", "9", "8", "*", "+" };
		PostfixExpr postfix = PostfixExpr(p0, p0 + sizeof(p0) / sizeof(p0[0]));

		Assert::IsTrue(pkb->isAssignExactPattern(6, infix));

		StringToken p1[] = { "(", "4", "+", "2", "/", "3", ")", "-", "1" };
		InfixExpr pattern1(p1, p1 + 9);

		Assert::IsTrue(pkb->isAssignContainsPattern(6, pattern1));
	}

	TEST_METHOD(TestPatternLong) {
		PKB * pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "a", "+", "(", "(", "b", "*", "(", "c", "-", "d", "*", "k", "/", "z", "+", "u", "*", "v", ")", "-", "x",
				"/", "10", "+", "8", ")", "-", "(", "3", "-", "z", ")", ")", "*", "20", "*", "k", "+", "c"};
		// a + ((b*(c-d*k/z+u*v) - x/10 + 8) - (3 - z)) * 20 * k + c
		InfixExpr infix = InfixExpr(arr, arr + sizeof(arr) / sizeof(arr[0]));
		pkb->putExprForStmt(7, pkb->infixToPostfix(infix));

		Assert::IsTrue(pkb->isAssignExactPattern(7, infix));

		StringToken p1[] = { "a", "+", "b" };
		InfixExpr pattern1(p1, p1 + 3);
		Assert::IsFalse(pkb->isAssignContainsPattern(7, pattern1));

		StringToken p2[] = { "c", "-", "d", "*", "k", "/", "z" };
		InfixExpr pattern2(p2, p2 + sizeof(p2) / sizeof(p2[0]));

/*		PostfixExpr p = pkb->infixToPostfix(pattern2);
		StringToken post = accumulate(p.begin(), p.end(), StringToken(""));
		Logger::WriteMessage(post.c_str());

		p = pkb->infixToPostfix(infix);
		post = accumulate(p.begin(), p.end(), StringToken(""));
		Logger::WriteMessage(post.c_str()); */

		Assert::IsTrue(pkb->isAssignContainsPattern(7, pattern2));

		StringToken p3[] = { "c", "-", "(", "d", "*", "k", "/", "z", ")", "+", "(", "u", "*", "v", ")"};
		InfixExpr pattern3(p3, p3 + sizeof(p3) / sizeof(p3[0]));
		Assert::IsTrue(pkb->isAssignContainsPattern(7, pattern3));

		StringToken p4[] = { "(", "d", "*", "k", "/", "z", ")", "+", "(", "u", "*", "v", ")" };
		InfixExpr pattern4(p4, p4 + sizeof(p4) / sizeof(p4[0]));
		Assert::IsFalse(pkb->isAssignContainsPattern(7, pattern4));

		StringToken p5[] = { "d", "*", "k" };
		InfixExpr pattern5(p5, p5 + sizeof(p5) / sizeof(p5[0]));
		Assert::IsTrue(pkb->isAssignContainsPattern(7, pattern5));

		StringToken p6[] = { "k", "/", "z" };
		InfixExpr pattern6(p6, p6 + sizeof(p6) / sizeof(p6[0]));
		Assert::IsFalse(pkb->isAssignContainsPattern(7, pattern6));
	}

	TEST_METHOD(TestPatternTricky) {
		PKB * pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = {"3", "+", "(", "3", "+", "3", "+", "3", ")", "+", "3" };

		InfixExpr infix = InfixExpr(arr, arr + sizeof(arr) / sizeof(arr[0]));
		pkb->putExprForStmt(6, pkb->infixToPostfix(infix));

		Assert::IsTrue(pkb->isAssignExactPattern(6, infix));

		StringToken p[] = { "3", "+", "3", "+", "3" };
		InfixExpr pattern(p, p + sizeof(p) / sizeof(p[0]));
		Assert::IsTrue(pkb->isAssignContainsPattern(6, pattern));

		StringToken p2[] = { "3", "+", "3", "+", "3", "+", "3" };
		InfixExpr pattern2(p2, p2 + sizeof(p2) / sizeof(p2[0]));
		Assert::IsFalse(pkb->isAssignContainsPattern(6, pattern2));
	}
};
}