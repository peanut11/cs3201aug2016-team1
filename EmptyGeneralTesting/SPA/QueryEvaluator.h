#pragma once

#include "ClausePatternObject.h"
#include "ClauseSuchThatObject.h"
#include "ClauseWithObject.h"
#include "DummyPKB.h"
#include "PKB.h"
#include "QueryResultProjector.h"
#include "QueryTable.h"
#include "ResultGridManager.h"
#include "ClauseSelectObject.h"
#include "SynonymTable.h"

class QueryEvaluator {
private:
	static QueryEvaluator* _instance;
    QueryEvaluator();

	PKB* pkb; 
	QueryTable* queryTable;
    ResultGridManager* resultManager;
    SynonymTable* synonymTable;

public:
	static QueryEvaluator* getInstance();
    static VarName to_var_name(VarIndex varIndex);

    ClausePatternObject evaluatePattern(ClausePatternObject patternObject); 
    ClauseSuchThatObject evaluateSuchThat(ClauseSuchThatObject suchThatRelObject);
	ClauseWithObject evaluateWith(ClauseWithObject withObject);
    std::vector<std::string> evaluate(QueryTable queryTable);
	std::vector<std::string> evaluateSelect(ClauseSelectObject ClauseSelectObject, bool relationshipHolds);
    ResultGridManager* populateResultGrids();

    // For unit tests
    PKB* getPKB();
    void setPKB(PKB* pkb);
    std::set<VarName> getValuesForSynonym(SynonymString);
};
