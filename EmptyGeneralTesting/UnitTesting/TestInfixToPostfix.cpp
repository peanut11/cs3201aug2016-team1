#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/Types.h"
#include <vector>
#include <string>
#include <iostream>

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
};
}