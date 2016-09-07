#pragma once

#include<stdio.h>
#include <iostream>
#include "QueryTable.h"
#include "PKB.h"

class QueryEvaluator {

	static QueryEvaluator *_instance;
	QueryTable queryTable;
	PKB *mPKB;

public:
	static QueryEvaluator *getInstance();
	PKB *getPKB();
	void evaluate(QueryTable queryTable);
	SuchThatRelObject evaluateSuchThat(SuchThatRelObject suchThatRelObject);
	void evaluateWith(WithObject withObject);
	void evaluatePattern(PatternObject patternObject);
	void evaluateSelect(SelectObject selectObject);
};