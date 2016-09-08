#pragma once

#include "PKB.h"
#include "ClausePatternObject.h"
#include "SelectObject.h"
#include "ClauseSuchThatObject.h"
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
	ClauseSuchThatObject evaluateSuchThat(ClauseSuchThatObject suchThatRelObject);
	void evaluate(QueryTable queryTable);
//	void evaluatePattern(PatternTypeObject patternObject);
	void evaluateSelect(SelectObject selectObject);
//	void evaluateWith(WithObject withObject);
};