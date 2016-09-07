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

std::vector<SuchThatRelObject> QueryTable::getSuchThats()
{
	return suchThats;
}

std::vector<WithObject> QueryTable::getWiths()
{
	return withs;
}

std::vector<PatternObject> QueryTable::getPatterns()
{
	return patterns;
}
