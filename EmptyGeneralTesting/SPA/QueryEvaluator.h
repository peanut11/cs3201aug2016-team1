#pragma once

#include<stdio.h>
#include <iostream>
#include "QueryTable.h"

class QueryEvaluator {

	static QueryEvaluator *_instance;
	QueryTable queryTable;

public:
	static QueryEvaluator *getInstance();
	void evaluate(QueryTable queryTable);

};