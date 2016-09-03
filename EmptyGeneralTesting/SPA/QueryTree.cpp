// Maintained by: Gerald
//
// Constructed by:
// - QueryProcessor
//
// Used by:
// - QueryEvaluator

#include "QueryTree.h" 

SelectObject QueryTree::getSelect()
{
	return select;
}

std::vector<ClauseObject> QueryTree::getClauses()
{
	return clauses;
}
