#pragma once
#include<stdio.h>
#include <iostream>
#include <list>

#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"

class QueryProcessor {

	static QueryProcessor *_instance;

	QueryPreProcessor *mPreProcessor;
	QueryEvaluator *mEvaluator;
	QueryResultProjector *mResultProjector;
	void init();

public:
	
	static QueryProcessor *getInstance();
	std::vector <std::string> evaluate(std::string queryString);
	QueryPreProcessor *getQueryPreProcessor();
	QueryEvaluator *getQueryEvaluator();
	QueryResultProjector *getQueryResultProjector();

	




};