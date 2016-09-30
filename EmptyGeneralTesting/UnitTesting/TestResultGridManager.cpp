#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ResultGridManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestResultGridManager) {

public:
    TEST_METHOD(TestResultGridManager_InitialiseSynonym) {
        ResultGridManager* resultManager = new ResultGridManager();
        std::set<StmtNumber> statements = { 1,3,5,6,7 };
        std::set<VarIndex> variables = { 0,1,2 };

        if (resultManager->initialiseSynonym("a", statements)) {
            Logger::WriteMessage("Added 'a' to result manager");
        }

        if (resultManager->initialiseSynonym("v", variables)) {
            Logger::WriteMessage("Added 'v' to result manager");
        }

        std::string expected = std::to_string(resultManager->getValuesForSynonym("a").size());
        Assert::AreEqual(std::to_string(statements.size()), expected);

        expected = std::to_string(resultManager->getValuesForSynonym("v").size());
        Assert::AreEqual(std::to_string(variables.size()), expected);

        delete resultManager;
    }
    };

}
