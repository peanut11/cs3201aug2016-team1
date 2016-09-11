#pragma once

#include "PKB.h"
#include "ClausePatternObject.h"
#include "SelectObject.h"
#include "ClauseSuchThatObject.h"
#include "ClauseWithObject.h"
#include "ResultsTable.h"
#include "QueryTable.h"
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

	void evaluate(QueryTable queryTable);
	ResultsTable *populateResultTable(SynonymTable *synonymTable);
	ClauseSuchThatObject evaluateSuchThat(ClauseSuchThatObject suchThatRelObject);
	ClauseWithObject evaluateWith(ClauseWithObject withObject);
	ClausePatternObject evaluatePattern(ClausePatternObject patternObject);
	std::vector<std::string> evaluateSelect(SelectObject selectObject, bool relationshipHolds);

};