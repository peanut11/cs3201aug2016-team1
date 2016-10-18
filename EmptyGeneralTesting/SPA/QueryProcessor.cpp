// Maintained by: Gerald & Joseph
//
// Facade of:
// - QueryPreprocessor
// - QueryEvaluator

#include "Exceptions.h"
#include "QueryProcessor.h"
#include "../AutoTester/source/AbstractWrapper.h"

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

			//if (AbstractWrapper::GlobalStop) {
			//	return std::vector<std::string>();
			//}


			// QueryEvaluator get result from PKB
			std::vector<std::string> evaluatedResults = this->getQueryEvaluator()->evaluate(this->getQueryPreProcessor()->getQueryTable());
			
			// QP pass result to QResultProjector for data representation
			return this->getQueryResultProjector()->evaluate(evaluatedResults);
		}

		return std::vector <std::string>();

	}
	catch (std::invalid_argument e) {
		//std::cout << e.what() << std::endl;
		return std::vector <std::string>();
	}
	catch (std::length_error e) {
		//std::cout << e.what() << std::endl;
		return std::vector <std::string>();
	}
	catch (std::out_of_range e) {
		//std::cout << e.what() << std::endl;
		return std::vector <std::string>();
	}
	catch (std::domain_error e) {
		//std::cout << e.what() << std::endl;
		return std::vector <std::string>();
	}
	catch (Exceptions e) {
		//std::cout << e.what() << std::endl;
		return std::vector <std::string>();
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

