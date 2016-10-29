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
	this->result.clearAll();
	this->suchThats.clear();
	this->patterns.clear();
	this->withs.clear();
}

void QueryTable::setResultBooelan(bool value) {
	this->result.setBoolean(value);
}

/*
ClauseSelectObject QueryTable::getSelect()
{
	return this->select;
}
*/
ClauseSelectResultObject QueryTable::getResult() {
	return this->result;
}

std::vector<ClauseSuchThatObject*>& QueryTable::getSuchThats()
{
	return this->suchThats;
}

std::vector<ClauseWithObject*> QueryTable::getWiths()
{
	return this->withs;
}

std::vector<ClausePatternObject*> QueryTable::getPatterns()
{
	return this->patterns;
}

std::string QueryTable::toString() {
	std::string str = "========== QUERY TABLE ===============\n";

	str.append("== Result object ==\n");
	str.append("size = " + std::to_string(this->result.size()) + "\n");
	//str.append(this->select.getSynonymString() + "\n");
	for (auto value : this->result.getClauseSelectObjectList()) {
		str.append(value.getSynonymString());
		if (value.getAttrType() != AttrType::AttrType::INVALID) {
			str.append("." + this->getAttrString(value.getAttrType()));
		}
		str.append(", ");
	}
	str.append("\n");

	if (this->result.getBoolean()) {
		str.append("BOOLEAN - true \n");
	}
	else {
		str.append("BOOLEAN - false\n");
	}

		
	str.append("== Such That ==\n");
	str.append("size = " + std::to_string(this->suchThats.size()) + "\n");
	for (auto value : this->suchThats) {

		str.append(getRelationshipString(value->getRelationshipType()) + ": ");

		if (value->getArgsOne()->getIsSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + getEntityString(value->getArgsOne()->getEntityType()) + ", " + value->getArgsOne()->getStringValue() + ", " + std::to_string(value->getArgsOne()->getIntegerValue()) + " | ");

		if (value->getArgsTwo()->getIsSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + getEntityString(value->getArgsTwo()->getEntityType()) + ", " + value->getArgsTwo()->getStringValue() + ", " + std::to_string(value->getArgsTwo()->getIntegerValue()) + "\n");

	}
	
	str.append("\n== Pattern ==\n");
	for (auto value : this->patterns) {

		if (value->getIsFirstArgSynonym()) {
			str.append("true");
		}
		else {
			str.append("false");
		}

		str.append(", " + value->getPatternSynonymArgument() + ", " + value->getFirstArgument() + ", " + value->getSecondArgument());

		if (value->getPatternType() == EntityType::IF) {
			str.append(", " + value->getThirdArgument());
		}

		str.append("\n");

	}

	str.append("\n== With ==\n");
	str.append("refType, synonym, str, int\n");
	for (auto value : this->withs) {
		str.append(getWithRefString(value->getRefObject1()->getRefType()) + ", " + getEntityString(value->getRefObject1()->getEntityType()) + ", " + value->getRefObject1()->getSynonym() + ", " + value->getRefObject1()->getStringValue() + ", " + std::to_string(value->getRefObject1()->getIntegerValue()));
		str.append(" | " + getWithRefString(value->getRefObject2()->getRefType()) + ", " + getEntityString(value->getRefObject2()->getEntityType()) + ", " + value->getRefObject2()->getSynonym() + ", " + value->getRefObject2()->getStringValue() + ", " + std::to_string(value->getRefObject2()->getIntegerValue()) + "\n");
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
	case MODIFIES_P:
		return "modifies_p";
	case USES:
		return "uses";
	case USES_P:
		return "uses_p";
	case CALLS:
		return "calls";
	case CALLS_STAR:
		return "calls*";
	case FOLLOWS:
		return "follows";
	case FOLLOWS_STAR:
		return "follows*";
	case PARENT:
		return "parent";
	case PARENT_STAR:
		return "parent*";
	case NEXT:
		return "next";
	case NEXT_STAR:
		return "next*";
	case AFFECTS:
		return "affects";
	case AFFECTS_STAR:
		return "affects*";
	default:
		return "";
	}
}

std::string QueryTable::getWithRefString(WithRefType type) {
	switch (type)
	{
	case ATTRREF:
		return "attrRef";
	case SYNONYM:
		return "synonym";
	case IDENTIFIER:
		return "IDENT";
	case INTEGER:
		return "INTEGER";
	default:
		return "-";
	}
}

std::string QueryTable::getAttrString(AttrType::AttrType type) {
	switch (type) {
	case AttrType::PROC_NAME:
		return "procName";

	case AttrType::STMT_NO:
		return "stmt#";

	case AttrType::VAR_NAME:
		return "varName";

	case AttrType::VALUE:
		return "value";
	}
	
}

bool QueryTable::insertSelectObject(ClauseSelectObject object) {
	try {
		this->result.insertClauseSelectObject(object);
		//this->select = object;
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

bool QueryTable::insertSuchThatObject(ClauseSuchThatObject* object) {
	// second returns the status of insertion

	try {
		 this->suchThats.push_back(object);
		 return true;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

bool QueryTable::insertWithObject(ClauseWithObject* object) {
	try {
		this->withs.push_back(object);
		return true;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

bool QueryTable::insertPatternObject(ClausePatternObject* object) {
	try {
		this->patterns.push_back(object);
		return true;
	}
	catch (std::runtime_error e) {
		return false;
	}
}

