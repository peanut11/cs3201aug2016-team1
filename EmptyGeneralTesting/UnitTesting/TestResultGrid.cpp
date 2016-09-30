#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ResultGrid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestResultGrid) {

public:
    TEST_METHOD(TestResultGrid_Constructor) {
        std::set<StmtNumber> statements = { 1,3,5,6,7 };
        std::set<VarIndex> variables = { 8,9,10 };
        ResultGrid stmtGrid = ResultGrid("a", statements);
        ResultGrid varGrid = ResultGrid("v", variables);

        Assert::AreEqual(std::to_string(5), std::to_string(stmtGrid.getValuesForSynonym("a").size()));
        Assert::AreEqual(std::to_string(3), std::to_string(varGrid.getValuesForSynonym("v").size()));
    }
    };

}
