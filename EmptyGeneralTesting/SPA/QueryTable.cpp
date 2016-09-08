// Maintained by: Gerald
//
// Constructed by:
// - QueryProcessor
//
// Used by:
// - QueryEvaluator

#include "QueryTable.h" 
#include <exception>

SelectObject QueryTable::getSelect()
{
	return this->select;
}

std::set<ClauseSuchThatObject> QueryTable::getSuchThats()
{
	return this->suchThats;
}

std::set<WithObject> QueryTable::getWiths()
{
	return this->withs;
}

std::set<ClausePatternObject> QueryTable::getPatterns()
{
	return this->patterns;
}

bool QueryTable::replaceSelectObject(SelectObject object) {
	try {
		this->select = object;
		return true;
	}
	catch (std::runtime_error e) {
		return false;
	}

}

bool QueryTable::insertSuchThatRelObject(ClauseSuchThatObject object) {
	// second returns the status of insertion
	try {
		return this->suchThats.insert(object).second;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

bool QueryTable::insertWithObject(WithObject object) {
	try {
		return this->withs.insert(object).second;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

bool QueryTable::insertPatternObject(ClausePatternObject object) {
	try {
		return this->patterns.insert(object).second;
	}
	catch (std::runtime_error e) {
		return false;
	}
}
