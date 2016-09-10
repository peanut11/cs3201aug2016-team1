#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/AssignTree.h"
#include "../SPA/Types.h"
#include <vector>
#include <string>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{		
	TEST_CLASS(TestAssignTree) {
	public:
		TEST_METHOD(TestAssignTreeFromProgLine) {
			static const std::string arr[] = { "x", "=", "12", "+", "13", "+", "y" };
			ProgLine progLine = ProgLine(arr, arr + sizeof(arr) / sizeof(arr[0]));

			AssignTree actual = AssignTree(progLine);

			Node* plus1 = new Node(PLUS);
			plus1->setLeft(new Node(CONSTANT, "12"));
			plus1->setRight(new Node(CONSTANT, "13"));
			Node* plus2 = new Node(PLUS);
			plus2->setLeft(plus1);
			plus2->setRight(new Node(VARIABLE, "y"));
			
			AssignTree expected = AssignTree("x", ExprTree(plus2));

			Logger::WriteMessage("Before\n");

			Logger::WriteMessage(AssignTree::toString(actual).c_str());
			Logger::WriteMessage("\n");
			Logger::WriteMessage(AssignTree::toString(expected).c_str());
			Logger::WriteMessage("\n");

			Logger::WriteMessage("After\n");

			Assert::IsTrue(AssignTree::equals(actual, expected));
		}

		TEST_METHOD(TestAssingTreeSimple) {
			static const std::string arr[] = { "x", "=", "y" };
			ProgLine progLine = ProgLine(arr, arr + sizeof(arr) / sizeof(arr[0]));

			AssignTree actual = AssignTree(progLine);

			Node* node = new Node(VARIABLE, "y");
			AssignTree expected = AssignTree("x", ExprTree(node));

			Logger::WriteMessage("Before\n");

			Logger::WriteMessage(AssignTree::toString(actual).c_str());
			Logger::WriteMessage("\n");
			Logger::WriteMessage(AssignTree::toString(expected).c_str());
			Logger::WriteMessage("\n");

			Logger::WriteMessage("After\n");

			Assert::IsTrue(AssignTree::toString(actual) == AssignTree::toString(expected));
		}
	};
}