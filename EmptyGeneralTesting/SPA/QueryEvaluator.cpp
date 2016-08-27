// Maintained by: Joseph
//
// Accessed by:
// - QueryProcessor
//
// Uses:
// - QueryTree
//   - QueryTNode
#include "QueryEvaluator.h"

QueryEvaluator *QueryEvaluator::_instance;

/*
Singleton
*/
QueryEvaluator *QueryEvaluator::getInstance()
{
	if (!_instance)
		_instance = new QueryEvaluator;
	return _instance;
}

/*
Evaluate QueryTree
*/
void QueryEvaluator::evaluate() {
	try {
		// do the evaluation (Basic Query Evaluator BQE) 
	}
	catch (std::runtime_error e) {
		throw e.what();
	}
}
