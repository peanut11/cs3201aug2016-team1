#pragma once

#include "Exceptions.h"
#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"

class QueryProcessor {
private:
	static QueryProcessor *_instance;
    QueryProcessor();

	QueryPreProcessor*    mPreProcessor;
	QueryEvaluator*       mEvaluator;
	QueryResultProjector* mResultProjector;


public:
	static QueryProcessor* getInstance();

	QueryPreProcessor*     getQueryPreProcessor();
	QueryEvaluator*        getQueryEvaluator();

};
