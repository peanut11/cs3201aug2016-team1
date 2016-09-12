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
QueryProcessor *QueryProcessor::getInstance()
{
	if (!_instance)
		_instance = new QueryProcessor;
	_instance->init();
	return _instance;
}

void QueryProcessor::init() {
	this->mPreProcessor = QueryPreProcessor::getInstance();
	this->mEvaluator = QueryEvaluator::getInstance();
	// do other initialization here
}

/*
Evaluate QueryTable
*/
bool QueryProcessor::evaluate(std::string queryString) {

	try {
		
		// Call QueryPreProcessor
		// Populate RelationshipTable
		// Validate the query string, call QueryValidator
		// QueryValidator verified and populate QueryTable
		// return true or false

		bool isQueryValid = this->getQueryPreProcessor()->isValidQuery(queryString);

		return isQueryValid;

		if (isQueryValid) {
			// QueryEvaluator get result from PKB
		}

	}
	catch (std::runtime_error e) {
		std::cout << e.what() << std::endl;
	}

}

QueryPreProcessor *QueryProcessor::getQueryPreProcessor() {
	return this->mPreProcessor;
}

QueryEvaluator *QueryProcessor::getQueryEvaluator() {
	return this->mEvaluator;
}

