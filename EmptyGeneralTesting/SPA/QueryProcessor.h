#pragma once
#include<stdio.h>
#include <iostream>

#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"

class QueryProcessor {

	static QueryProcessor *_instance;

	QueryPreProcessor *mPreProcessor;
	QueryEvaluator *mEvaluator;

	void init();

public:
	
	static QueryProcessor *getInstance();
	bool evaluate(std::string queryString);
	QueryPreProcessor *getQueryPreProcessor();
	QueryEvaluator *getQueryEvaluator();


	




};