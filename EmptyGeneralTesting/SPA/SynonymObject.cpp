#include "SynonymObject.h"

SynonymObject::SynonymObject() {}

SynonymObject::SynonymObject(EntityType type, SynonymString synonym) {
	this->type = type;
	this->synonym = synonym;
}

EntityType SynonymObject::getType() {
	return this->type;
}

std::set<SynonymString> SynonymObject::getSetString() {
	return this->mSetString;
}

std::set<SynonymValue> SynonymObject::getSetInt() {
	return this->mSetInt;
}

SynonymString SynonymObject::getSynonym() {
	return this->synonym;
}

SynonymString SynonymObject::getTypeString() {
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
	case STMTLST:
		return "stmtLst";
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

void SynonymObject::replaceSet(std::set<SynonymString> newSet) {
	this->mSetString.clear();
	this->mSetString.insert(newSet.begin(), newSet.end());
}

void SynonymObject::replaceSet(std::set<SynonymValue> newSet) {
	this->mSetInt.clear();
	this->mSetInt.insert(newSet.begin(), newSet.end());
}

void SynonymObject::insertSet(std::set<SynonymString> updatedSet) {
	this->mSetString.insert(updatedSet.begin(), updatedSet.end());
}

void SynonymObject::insertSet(std::set<SynonymValue> updatedSet) {
	this->mSetInt.insert(updatedSet.begin(), updatedSet.end());
}

void SynonymObject::insertSet(SynonymString variable) {
	this->mSetString.insert(variable);
}

void SynonymObject::insertSet(SynonymValue stmtNum) {
	this->mSetInt.insert(stmtNum);
}
