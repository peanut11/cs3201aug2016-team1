#pragma once

#include "PKB.h"

#include "ClauseObject.h"
#include "PatternObject.h"
#include "SelectObject.h"
#include "SuchThatRelObject.h"
#include "WithObject.h"

#include "QueryTable.h"
#include "SynonymTable.h"

class QueryEvaluator {
private:
	static QueryEvaluator *_instance;
	QueryEvaluator();

	PKB *mPKB; 
	QueryTable queryTable;
	SynonymTable *mSynonymTable;

public:
	static QueryEvaluator *getInstance();
	PKB *getPKB();

	SynonymTable *getSynonymTable();
	SuchThatRelObject evaluateSuchThat(SuchThatRelObject suchThatRelObject);
	void evaluate(QueryTable queryTable);
	void evaluatePattern(PatternObject patternObject);
	void evaluateSelect(SelectObject selectObject);
	void evaluateWith(WithObject withObject);
};