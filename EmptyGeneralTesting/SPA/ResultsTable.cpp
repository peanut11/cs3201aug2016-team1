#include "ResultsTable.h"

ResultsObject ResultsTable::getObject(SynonymString synonym) {
	for (auto value : this->resultsObject) {
		if (value.getSynonym().compare(synonym) == 0) {
			return value;
		}
	}
}

std::set<ResultsObject> ResultsTable::getObjects()
{
	return this->resultsObject;
}

std::set<SynonymString> ResultsTable::getSetString(SynonymString synonym)
{
	if (this->contains(synonym)) {
		return this->getObject(synonym).getSetString();
	}

	return std::set<SynonymString>();
}

std::set<SynonymValue> ResultsTable::getSetInt(SynonymString synonym)
{
	if (this->contains(synonym)) {
		return this->getObject(synonym).getSetInt();
	}

	return std::set<SynonymValue>();
}

bool ResultsTable::contains(ResultsObject object) {
	for (auto value : this->resultsObject) {
		if (value.getSynonym() == object.getSynonym()) {
			return true;
		}
	}
	return false;
}

bool ResultsTable::contains(SynonymString synonym) {
	for (auto value : this->resultsObject) {
		if (value.getSynonym().compare(synonym) == 0) {
			return true;
		}
	}
	return false;
}

int ResultsTable::size() {
	return resultsObject.size();
}

bool ResultsTable::insert(ResultsObject object) {

	if (contains(object)) {
		throw std::runtime_error("ResultTable insert duplicate object synonym = " + object.getSynonym());
		return false;
	}

	resultsObject.insert(object);
	return true; 
}

bool ResultsTable::replaceSet(SynonymString synonym, std::set<SynonymString> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).replaceSet(newSet);
		return true;
	}
	return false;
}

bool ResultsTable::replaceSet(SynonymString synonym, std::set<ProcStmtVarIndex> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).replaceSet(newSet);
		return true;
	}
	return false;
}

bool ResultsTable::insertSet(SynonymString synonym, std::set<SynonymString> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).insertSet(newSet);
		return true;
	}
	return false;
}

bool ResultsTable::insertSet(SynonymString synonym, std::set<SynonymValue> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).insertSet(newSet);
		return true;
	}
	return false;
}