#pragma once

#include "ClausePatternObject.h"
#include "ClauseSelectObject.h"
#include "ClauseSuchThatObject.h"
#include "ClauseWithObject.h"
#include "DummyPKB.h"
#include "PKB.h"
#include "QueryOptimization.h"
#include "QueryResultProjector.h"
#include "QueryTable.h"
#include "ResultGridManager.h"
#include "StringTokenizer.h"
#include "SynonymTable.h"

class QueryEvaluator {
private:
	static QueryEvaluator* _instance;
    QueryEvaluator();

	PKB* pkb; 
	QueryTable* queryTable;
	QueryOptimization* queryOptimizer;
	ResultGridManager* resultManager;
    SynonymTable* synonymTable;

    std::vector<StringToken> to_tokens(std::string str);

public:
	static QueryEvaluator* getInstance();
    static VarName to_var_name(VarIndex varIndex);
	static ProcName to_proc_name(ProcIndex procIndex);
	static ProcName from_call_to_proc_name(ProcIndex procIndex);

    ClausePatternObject* evaluatePattern(ClausePatternObject* patternObject, bool isStopEvaluation);
    ClauseSuchThatObject* evaluateSuchThat(ClauseSuchThatObject* suchThatRelObject, bool isStopEvaluation);
	ClauseWithObject* evaluateWith(ClauseWithObject* withObject, bool isStopEvaluation);
    std::vector<std::string> evaluate(QueryTable queryTable);
//	std::vector<std::string> evaluateSelect(ClauseSelectObject ClauseSelectObject, bool relationshipHolds);
	std::vector<std::string> evaluateSelect(ClauseSelectResultObject ClauseSelectResultObject, bool relationshipHolds);
    ResultGridManager* populateResultGrids();
	QueryOptimization* getQueryOptimizer();

    // For unit tests
    PKB* getPKB();
    void setPKB(PKB* pkb);
    std::set<VarName> getValuesForSynonym(SynonymString);
};
