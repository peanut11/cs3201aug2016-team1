// Maintained by: Gerald
//
// Constructed by:
// - QueryProcessor
//
// Used by:
// - QueryEvaluator

#include "QueryTable.h" 
#include <exception>

QueryTable::QueryTable(){}

QueryTable::~QueryTable(){}

SelectObject QueryTable::getSelect()
{
	return this->select;
}

std::vector<ClauseSuchThatObject> QueryTable::getSuchThats()
{
	return this->suchThats;
}

std::vector<ClauseWithObject> QueryTable::getWiths()
{
	return this->withs;
}

std::vector<ClausePatternObject> QueryTable::getPatterns()
{
	return this->patterns;
}

std::string QueryTable::toString() {
	std::string str = "========== QUERY TABLE ===============\n";

	//str.append("== Select object ==\n");
	//str.append(this->select.get
	str.append("== Such That ==\n");
	for (auto value : this->suchThats) {

		if (value.getArgsOne().getIsSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + value.getArgsOne().getStringValue() + ", " + std::to_string(value.getArgsOne().getIntegerValue()) + " | ");

		if (value.getArgsTwo().getIsSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + value.getArgsTwo().getStringValue() + ", " + std::to_string(value.getArgsTwo().getIntegerValue()) + "\n");

	}
	return str;
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

bool QueryTable::insertSuchThatObject(ClauseSuchThatObject object) {
	// second returns the status of insertion
	try {
		 this->suchThats.push_back(object);
		 return true;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

bool QueryTable::insertWithObject(ClauseWithObject object) {
	try {
		this->withs.push_back(object);
		return true;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

bool QueryTable::insertPatternObject(ClausePatternObject object) {
	try {
		this->patterns.push_back(object);
		return true;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

