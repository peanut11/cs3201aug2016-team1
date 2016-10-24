// Maintained by: Gerald & Joseph
//
// Facade of:
// - QueryPreprocessor
// - QueryEvaluator

#include "QueryProcessor.h"

QueryProcessor *QueryProcessor::_instance;

/*
Singleton
*/
QueryProcessor *QueryProcessor::getInstance() {
    if (!_instance) {
        _instance = new QueryProcessor();
    }

	return _instance;
}

QueryProcessor::QueryProcessor() {
	this->mPreProcessor = QueryPreProcessor::getInstance();
	this->mEvaluator = QueryEvaluator::getInstance();
	this->mResultProjector = new QueryResultProjector();
	// Do other initialization here
    isEvaluated = false;
}

void QueryProcessor::observeGlobalStop(volatile bool* stop) {
    while (!isEvaluated) {
        if (*stop) {
            mEvaluator->isGlobalStop = true;
            return;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void QueryProcessor::startObserver(volatile bool* stop) {
    t = std::thread(&QueryProcessor::observeGlobalStop, this, stop);
}

/*
Evaluate QueryTable
*/
std::vector<std::string> QueryProcessor::evaluate(std::string queryString) {
	try {
		// Call QueryPreProcessor
		// Populate RelationshipTable
		// Validate the query string, call QueryValidator
		// QueryValidator verified and populate QueryTable
		// return true or false
		bool isQueryValid = this->getQueryPreProcessor()->isValidQuery(queryString);

		if (isQueryValid) {
			// QueryEvaluator get result from PKB
			std::vector<std::string> evaluatedResults = this->getQueryEvaluator()->evaluate(this->getQueryPreProcessor()->getQueryTable());
            isEvaluated = true;

			// QP pass result to QResultProjector for data representation
			return this->mResultProjector->evaluate(evaluatedResults);
		}
	} catch (std::invalid_argument e) {
		//std::cout << e.what() << std::endl;
	} catch (std::length_error e) {
		//std::cout << e.what() << std::endl;
	} catch (std::out_of_range e) {
		//std::cout << e.what() << std::endl;
	} catch (std::domain_error e) {
		//std::cout << e.what() << std::endl;
	} catch (Exceptions e) {
		//std::cout << e.what() << std::endl;
    }

    isEvaluated = true;
    return std::vector <std::string>();
}

QueryPreProcessor *QueryProcessor::getQueryPreProcessor() {
	return this->mPreProcessor;
}

QueryEvaluator *QueryProcessor::getQueryEvaluator() {
	return this->mEvaluator;
}
