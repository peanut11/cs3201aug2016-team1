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

//QueryTable::~QueryTable(){}

void QueryTable::clearAll() {
	this->suchThats.clear();
	this->patterns.clear();
	this->withs.clear();
}

SelectObject QueryTable::getSelect()
{
	return this->select;
}

std::vector<ClauseSuchThatObject>& QueryTable::getSuchThats()
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

	str.append("== Select object ==\n");
	str.append(this->select.getSynonymString() + "\n");
	
		
	str.append("== Such That ==\n");
	str.append("size = " + std::to_string(this->suchThats.size()) + "\n");
	for (auto value : this->suchThats) {

		str.append(getRelationshipString(value.getRelationshipType()) + ": ");

		if (value.getArgsOne().getIsSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + getEntityString(value.getArgsOne().getEntityType()) + ", " + value.getArgsOne().getStringValue() + ", " + std::to_string(value.getArgsOne().getIntegerValue()) + " | ");

		if (value.getArgsTwo().getIsSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + getEntityString(value.getArgsTwo().getEntityType()) + ", " + value.getArgsTwo().getStringValue() + ", " + std::to_string(value.getArgsTwo().getIntegerValue()) + "\n");

	}
	
	str.append("\n== Pattern ==\n");
	for (auto value : this->patterns) {

		if (value.getIsFirstArgSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + value.getPatternSynonymArgument() + ", " + value.getFirstArgument() + ", " + value.getSecondArgument() + "\n");

	}
	
	return str;
}

std::string QueryTable::getEntityString(EntityType type) {
	switch (type) {
	case PROCEDURE:
		return "procedure";
	case ASSIGN:
		return "assign";
	case IF:
		return "if";
	case WHILE:
		return "while";
	case STMT:
		return "stmt";
	case VARIABLE:
		return "variable";
	case CONSTANT:
		return "constant";
	case PROGRAM_LINE:
		return "prog_line";
	case CALL:
		return "call";
	case WILDCARD:
		return "wc";
	default:
		return "";
	}
}

std::string QueryTable::getRelationshipString(RelationshipType type) {
	switch (type) {
	case MODIFIES:
		return "modifies";
	case USES:
		return "uses";
	case CALLS:
		return "calls";
	case FOLLOWS:
		return "follows";
	case FOLLOWS_STAR:
		return "follows*";
	case PARENT:
		return "parent";
	case PARENT_STAR:
		return "parent*";
	default:
		return "";
	}
}

bool QueryTable::replaceSelectObject(SelectObject object) {
	try {
		this->select = object;
		/*
		this->select.setAttrType(object.getAttrType());
		this->select.setEntityType(object.getEntityType());
		this->select.setSynonymString(object.getSynonymString());
		this->select.setIsBoolean(object.getBoolean());
		*/
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

