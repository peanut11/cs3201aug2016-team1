#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/AbstractSyntaxTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{		
	TEST_CLASS(TestAST)
	{
	public:
		
		TEST_METHOD(TestSingleton)
		{
			AbstractSyntaxTree* ast0 = AbstractSyntaxTree::getInstance();
			AbstractSyntaxTree* ast1 = AbstractSyntaxTree::getInstance();
			Assert::IsTrue(ast0 == ast1);
		}

	};
}