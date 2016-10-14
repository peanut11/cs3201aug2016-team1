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

        Assert::AreEqual(size_t(5), stmtGrid.getValuesForSynonym("a").size());
        Assert::AreEqual(size_t(3), varGrid.getValuesForSynonym("v").size());
    }

    TEST_METHOD(TestResultGrid_MergeGrid) {
        std::set<StmtNumber> statements = { 1,3,5,6,7 };
        std::set<VarIndex> variables = { 8,9,10 };
        ResultGrid stmtGrid = ResultGrid("a", statements);
        ResultGrid varGrid = ResultGrid("v", variables);

        SynonymTuple syns = SynonymTuple("a", "v");
        ValueTupleSet vals = { ValueTuple(1,8), ValueTuple(3,9), ValueTuple(5,10) };

        stmtGrid.mergeGrid(&varGrid, syns, vals);
        Assert::AreEqual(size_t(3), stmtGrid.getValuesForSynonym("a").size());
        Assert::AreEqual(size_t(3), stmtGrid.getValuesForSynonym("v").size());
        Assert::AreEqual(size_t(3), stmtGrid.getValuesForSynonymTuple(syns).size());
    }

    };
}
