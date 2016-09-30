#pragma once

#include "ClausePatternObject.h"
#include "ClauseSuchThatObject.h"
#include "ClauseWithObject.h"
#include "DummyPKB.h"
#include "PKB.h"
#include "QueryResultProjector.h"
#include "QueryTable.h"
#include "SelectObject.h"
#include "SynonymTable.h"

class QueryEvaluator {
private:
	static QueryEvaluator *_instance;

	PKB *mPKB; 
	QueryTable *queryTable;
	SynonymTable *mSynonymTable;
	ResultsTable resultsTable;

public:
	static QueryEvaluator *getInstance();
	PKB *getPKB();
	SynonymTable *getSynonymTable();
	ResultsTable getResultsTable();
	void *setPKB(PKB *pkb);

	std::vector <std::string> evaluate(QueryTable queryTable);
	ResultsTable *populateResultTable(SynonymTable *synonymTable);
	ClauseSuchThatObject evaluateSuchThat(ClauseSuchThatObject suchThatRelObject);
	ClauseWithObject evaluateWith(ClauseWithObject withObject);
	ClausePatternObject evaluatePattern(ClausePatternObject patternObject);
	std::vector<std::string> evaluateSelect(SelectObject selectObject, bool relationshipHolds);
};
