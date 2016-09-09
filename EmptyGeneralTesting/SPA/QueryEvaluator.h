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
	ResultsTable resultsTable;
	SynonymTable *mSynonymTable;

public:
	static QueryEvaluator *getInstance();
	PKB *getPKB();

	SynonymTable *getSynonymTable();
	void evaluate(QueryTable queryTable);
	ClauseSuchThatObject evaluateSuchThat(ClauseSuchThatObject suchThatRelObject);
	ClauseWithObject evaluateWith(ClauseWithObject withObject);
	ClausePatternObject evaluatePattern(ClausePatternObject patternObject);
	bool evaluateSelect(SelectObject selectObject, bool relationshipHolds);

};