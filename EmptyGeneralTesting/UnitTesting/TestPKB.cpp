#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestPKB) {

public:
    TEST_METHOD(TestPKB_StmtTable) {
        PKB* pkb = PKB::getInstance();

        // Follows(1, 2)
        pkb->putStmtForStmt(StmtNumber(1), FOLLOWS, StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(FOLLOWS, StmtNumber(1)).begin(), StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(StmtNumber(2), FOLLOWS).begin(), StmtNumber(1));
        Assert::IsTrue(pkb->is(FOLLOWS, StmtNumber(1), StmtNumber(2)));
        Assert::IsFalse(pkb->is(FOLLOWS, StmtNumber(2), StmtNumber(1)));

        // Parent(1, 2)
        pkb->putStmtForStmt(StmtNumber(1), PARENT, StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(PARENT, StmtNumber(1)).begin(), StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(StmtNumber(2), PARENT).begin(), StmtNumber(1));

        // Next(1, 2)
        pkb->putStmtForStmt(StmtNumber(1), NEXT, StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(NEXT, StmtNumber(1)).begin(), StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(StmtNumber(2), NEXT).begin(), StmtNumber(1));

        // Parent*(1, 2)
        pkb->putStmtForStmt(StmtNumber(1), PARENT_STAR, StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(PARENT_STAR, StmtNumber(1)).begin(), StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(StmtNumber(2), PARENT_STAR).begin(), StmtNumber(1));

        // FOLLOWS*(1, 2)
        pkb->putStmtForStmt(StmtNumber(1), FOLLOWS_STAR, StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(FOLLOWS_STAR, StmtNumber(1)).begin(), StmtNumber(2));
        Assert::AreEqual(*pkb->getStmtsByStmt(StmtNumber(2), FOLLOWS_STAR).begin(), StmtNumber(1));
    }

    TEST_METHOD(TestPKB_StmtTable_InternalUseError) {
        PKB* pkb = PKB::getInstance();

		try {
			pkb->putStmtForStmt(StmtNumber(2), FOLLOWED_BY, StmtNumber(1));
            Assert::Fail();
		} catch (std::invalid_argument e) {
			Assert::AreEqual(e.what(), Exception::INTERNAL_USE_ERROR.what());
		}

		try {
			pkb->putStmtForStmt(StmtNumber(2), PARENT_OF, StmtNumber(1));
            Assert::Fail();
		} catch (std::invalid_argument e) {
			Assert::AreEqual(e.what(), Exception::INTERNAL_USE_ERROR.what());
		}

		try {
			pkb->putStmtForStmt(StmtNumber(2), FOLLOWED_BY_STAR, StmtNumber(1));
            Assert::Fail();
		} catch (std::invalid_argument e) {
			Assert::AreEqual(e.what(), Exception::INTERNAL_USE_ERROR.what());
		}
		
		try {
			pkb->putStmtForStmt(StmtNumber(2), PARENT_OF_STAR, StmtNumber(1));
            Assert::Fail();
		} catch (std::invalid_argument e) {
			Assert::AreEqual(e.what(), Exception::INTERNAL_USE_ERROR.what());
		}

		try {
			pkb->getStmtsByStmt(StmtNumber(1), FOLLOWED_BY);
            Assert::Fail();
		} catch (std::invalid_argument e) {
			Assert::AreEqual(e.what(), Exception::INTERNAL_USE_ERROR.what());
		}

		try {
			pkb->getStmtsByStmt(FOLLOWED_BY, StmtNumber(1));
            Assert::Fail();
		} catch (std::invalid_argument e) {
			Assert::AreEqual(e.what(), Exception::INTERNAL_USE_ERROR.what());
		}
	}
	};
}
