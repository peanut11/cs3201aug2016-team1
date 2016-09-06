// Maintained by: Gerald
//
// Constructed by:
// - QueryProcessor
//
// Used by:
// - QueryEvaluator

#include "QueryTable.h" 

SelectObject QueryTable::getSelect()
{
	return select;
}

std::vector<ClauseObject> QueryTable::getClauses()
{
	return clauses;
}
