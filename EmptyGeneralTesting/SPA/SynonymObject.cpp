#include "SynonymObject.h"


SynonymObject::SynonymObject(EntityType type, std::string synonym) {
	this->type = type;
	this->synonym = synonym;
}

EntityType SynonymObject::getType() {
	return this->type;
}

std::string SynonymObject::getSynonym() {
	return this->synonym;
}

std::string SynonymObject::getTypeString() {
	switch (this->type) {
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
	default:
		return "";
	}
}

void SynonymObject::insertSet(std::string variable) {
	this->mSetString.insert(variable);
}

void SynonymObject::insertSet(int stmtNum) {
	this->mSetInt.insert(stmtNum);
}
