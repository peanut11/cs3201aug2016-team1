#pragma once

#include <chrono>
#include <thread>

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

    std::thread t;
    bool isEvaluated;
    void observeGlobalStop(volatile bool* stop);

public:
	static QueryProcessor* getInstance();

	QueryPreProcessor*     getQueryPreProcessor();
	QueryEvaluator*        getQueryEvaluator();

    void startObserver(volatile bool* stop);
    std::vector<std::string> evaluate(std::string queryString);
};
