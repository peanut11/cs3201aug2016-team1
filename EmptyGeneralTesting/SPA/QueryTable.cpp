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

std::set<SuchThatRelObject> QueryTable::getSuchThats()
{
	return suchThats;
}

std::set<WithObject> QueryTable::getWiths()
{
	return withs;
}

std::set<PatternObject> QueryTable::getPatterns()
{
	return patterns;
}
