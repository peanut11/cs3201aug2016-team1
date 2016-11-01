#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Frontend.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestFrontend) {
public:

    TEST_METHOD(TestFrontend_ProcedureDeletion) {
        PKB::getInstance()->clear();

        Frontend frontend = Frontend();
        try {
            frontend.parse("procedure_Deletion.txt");
        } catch (StmtNumber s) {
            Assert::AreEqual((StmtNumber)0, s);
        }
    }
    };
}
