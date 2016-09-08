#include "ResultsObject.h"

ResultsObject::ResultsObject() {}

ResultsObject::ResultsObject(std::string synonym)
{
	this->synonym = synonym;
}

std::set<SynonymString> ResultsObject::getSetString() {
	return this->mSetString;
}

std::set<SynonymValue> ResultsObject::getSetInt() {
	return this->mSetInt;
}

SynonymString ResultsObject::getSynonym() {
	return this->synonym;
}

void ResultsObject::replaceSet(std::set<SynonymString> newSet) {
	this->mSetString.clear();
	this->mSetString.insert(newSet.begin(), newSet.end());
}

void ResultsObject::replaceSet(std::set<SynonymValue> newSet) {
	this->mSetInt.clear();
	this->mSetInt.insert(newSet.begin(), newSet.end());
}

void ResultsObject::insertSet(std::set<SynonymString> updatedSet) {
	this->mSetString.insert(updatedSet.begin(), updatedSet.end());
}

void ResultsObject::insertSet(std::set<SynonymValue> updatedSet) {
	this->mSetInt.insert(updatedSet.begin(), updatedSet.end());
}

/*
void ResultsObject::insertSet(SynonymString variable) {
	this->mSetString.insert(variable);
}

void ResultsObject::insertSet(SynonymValue stmtNum) {
	this->mSetInt.insert(stmtNum);
}
*/