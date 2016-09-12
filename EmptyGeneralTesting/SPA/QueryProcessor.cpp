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
	this->mResultProjector = new QueryResultProjector();
	// do other initialization here
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
	//		std::vector<std::string> evaluatedResults = this->getQueryEvaluator()->evaluate(queryTable);
	//		return this->getQueryResultProjector()->evaluate(evaluatedResults);
		}


		return std::vector <std::string>();

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

QueryResultProjector * QueryProcessor::getQueryResultProjector()
{
	return this->mResultProjector;
}

