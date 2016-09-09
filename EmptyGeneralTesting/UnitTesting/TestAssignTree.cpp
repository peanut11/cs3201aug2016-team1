#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/AssignTree.h"
#include "../SPA/Types.h"
#include "../SPA/PKB.h"
#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{		
	TEST_CLASS(TestAssignTree) {
	public:
		TEST_METHOD(TestAssignTreeFromProgLine) {
			PKB *pkb = PKB::getInstance();

			static const std::string arr[] = { "x", "=", "12", "+", "13", "+", "y" };
			ProgLine progLine = ProgLine(arr, arr + sizeof(arr) / sizeof(arr[0]));

			AssignTree actual = AssignTree(progLine);

			Node* plus1 = new Node(PLUS);
			plus1->setLeft(new Node(CONSTANT, pkb->getVarIndex("12")));
			plus1->setRight(new Node(CONSTANT, pkb->getVarIndex("13")));
			Node* plus2 = new Node(PLUS);
			plus2->setLeft(plus1);
			plus2->setRight(new Node(VARIABLE, pkb->getVarIndex("y")));
			
			AssignTree expected = AssignTree(pkb->getVarIndex("x"), ExprTree(plus2));

			AssignTree::print(actual);
			AssignTree::print(expected);

			Assert::IsTrue(AssignTree::equals(actual, expected));
		}
	};
}