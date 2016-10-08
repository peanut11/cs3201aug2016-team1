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
	TEST_CLASS(TestInfixToPostfix) {
public:
	TEST_METHOD(TestSimpleInfix) {
		PKB * pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "(", "3", "+", "x", "-", "y", "+", "gama", ")", "*", "y", "-", "alpha", "/", "beta" };
		InfixExpr infix = InfixExpr(arr, arr + sizeof(arr) / sizeof(arr[0]));

		StringToken b[] = { "3", "x", "+", "y", "-", "gama", "+", "y", "*", "alpha", "beta", "/", "-" };
		PostfixExpr expectedPostfix = PostfixExpr(b, b + sizeof(b) / sizeof(b[0]));

		PostfixExpr actualPostfix = pkb->infixToPostfix(infix);

		Assert::IsTrue(expectedPostfix == actualPostfix);
	}

	TEST_METHOD(TestLongerInfix) {
		PKB * pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken arr[] = { "3", "+", "5", "*", "(", "4", "+", "2", "/", "3", "-", "1", ")", "+", "7" , "+", "9", "*", "8"};
		InfixExpr infix = InfixExpr(arr, arr + sizeof(arr) / sizeof(arr[0]));

		StringToken b[] = { "3", "5", "4", "2", "3", "/", "+", "1", "-", "*", "+", "7", "+", "9", "8", "*", "+" };
		PostfixExpr expectedPostfix = PostfixExpr(b, b + sizeof(b) / sizeof(b[0]));
		PostfixExpr actualPostfix = pkb->infixToPostfix(infix);

		Assert::IsTrue(expectedPostfix == actualPostfix);
	}

	TEST_METHOD(TestInfix2) {
		PKB * pkb = PKB::getInstance();
		pkb->clear();
		pkb = PKB::getInstance();

		StringToken ar2[] = { "a", "+", "b", "*", "(", "c", "-", "d", "*", "k", "/", "z", "+", "u", "*", "v", ")" };
		InfixExpr inf2 = InfixExpr(ar2, ar2 + sizeof(ar2) / sizeof(ar2[0]));

/*		PostfixExpr p = pkb->infixToPostfix(inf2);
		StringToken post = accumulate(p.begin(), p.end(), StringToken(""));

		Logger::WriteMessage(post.c_str()); */

		StringToken b[] = { "a", "b", "c", "d", "k", "*", "z", "/", "-", "u", "v", "*", "+", "*", "+" };
		PostfixExpr expectedPostfix = PostfixExpr(b, b + sizeof(b) / sizeof(b[0]));
		PostfixExpr actualPostfix = pkb->infixToPostfix(inf2);

/*		post = accumulate(expectedPostfix.begin(), expectedPostfix.end(), StringToken(""));

		Logger::WriteMessage(post.c_str()); */

		Assert::IsTrue(expectedPostfix == actualPostfix);
	}
};
}