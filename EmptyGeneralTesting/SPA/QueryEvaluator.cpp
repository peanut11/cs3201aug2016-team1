// Maintained by: Joseph
//
// Accessed by:
// - QueryProcessor
//
// Uses:
// QueryTable
// ClauseObject
// SelectObject

#include "QueryEvaluator.h"
#include "QueryTable.h"
#include "ClauseObject.h"
#include "SelectObject.h"

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
void QueryEvaluator::evaluate(QueryTable queryTable) {
	try {
		// do the evaluation (Basic Query Evaluator BQE) 
		this->queryTable = queryTable;

		// gets all the clauses and select objects
		std::vector<ClauseObject> clauses = queryTable.getClauses();
		SelectObject select = queryTable.getSelect();

		// iterate the clauses objects
		for (std::vector<ClauseObject>::const_iterator it = clauses.begin(); it < clauses.end(); it++) {
			it->execute();
		}
	}
	catch (std::runtime_error e) {
		throw e.what();
	}
}
