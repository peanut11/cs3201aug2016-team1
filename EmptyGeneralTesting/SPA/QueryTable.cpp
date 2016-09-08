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
	return this->select;
}

std::set<SuchThatRelObject> QueryTable::getSuchThats()
{
	return this->suchThats;
}

std::set<WithObject> QueryTable::getWiths()
{
	return this->withs;
}

std::set<PatternObject> QueryTable::getPatterns()
{
	return this->patterns;
}
