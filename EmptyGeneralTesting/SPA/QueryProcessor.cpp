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
void QueryProcessor::evaluate() {

	try {
		// call QueryEvaulator and evaluate QueryTable
		this->mEvaluator->evaluate();
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

