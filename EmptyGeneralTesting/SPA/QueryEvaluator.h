#pragma once

#include<stdio.h>
#include <iostream>
#include "QueryTable.h"
#include "PKB.h"
#include "SynonymTable.h"

class QueryEvaluator {

	static QueryEvaluator *_instance;
	QueryTable queryTable;
	PKB *mPKB;
	SynonymTable *mSynonymTable;

public:
	static QueryEvaluator *getInstance();
	PKB *getPKB();
	SynonymTable *getSynonymTable();
	void evaluate(QueryTable queryTable);
	SuchThatRelObject evaluateSuchThat(SuchThatRelObject suchThatRelObject);
	void evaluateWith(WithObject withObject);
	void evaluatePattern(PatternObject patternObject);
	void evaluateSelect(SelectObject selectObject);
};