#include "SynonymTable.h"

SynonymTable *SynonymTable::_instance;

/*
Singleton
*/
SynonymTable *SynonymTable::getInstance()
{
	if (!_instance)
		_instance = new SynonymTable;
	return _instance;
}

void SynonymTable::clearAll() {
	synonymObjects.clear();
}

std::set<SynonymString> SynonymTable::getSetString(SynonymString synonym) {
	if (synonym.compare("_") == 0) {
		std::set<SynonymString> fullSet = std::set<SynonymString>();

		for (auto value : this->synonymObjects) {
			fullSet.insert(value.getSetString().begin(), value.getSetString().end());
		}

		return fullSet;
	}

	if (this->contains(synonym)) {
		return this->getObject(synonym).getSetString();
	}

	return std::set<SynonymString>();
}

std::set<SynonymValue> SynonymTable::getSetInt(SynonymString synonym) {
	if (synonym.compare("_") == 0) {
		std::set<SynonymValue> fullSet = std::set<SynonymValue>();

		for (auto value : this->synonymObjects) {
			fullSet.insert(value.getSetInt().begin(), value.getSetInt().end());
		}

		return fullSet;
	}

	if (this->contains(synonym)) {
		return this->getObject(synonym).getSetInt();
	}

	return std::set<SynonymValue>();
}

/*
Insert a new object into RelObjects vector
Return the index of the new object*/
bool SynonymTable::insert(SynonymObject object) {

	if (contains(object)) {
		throw std::runtime_error("SynonymTable insert duplicate object synonym = " + object.getSynonym());
		return false;
	}

	synonymObjects.push_back(object); 
	return true; // insert successfully;
	//return synonymObjects.size() - 1; // last index of the object
}

bool SynonymTable::replaceSet(SynonymString synonym, std::set<SynonymString> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).replaceSet(newSet);
		return true;
	}
	return false;
}

bool SynonymTable::replaceSet(SynonymString synonym, std::set<ProcStmtVarIndex> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).replaceSet(newSet);
		return true;
	}
	return false;
}

bool SynonymTable::insertSet(SynonymString synonym, std::set<SynonymString> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).insertSet(newSet);
		return true;
	}
	return false;
}

bool SynonymTable::insertSet(SynonymString synonym, std::set<SynonymValue> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).insertSet(newSet);
		return true;
	}
	return false;
}

/*
Check if synonym is inside the table
Return	@true	if contains
@false	does not contains
*/
bool SynonymTable::contains(SynonymObject object) {
	for (auto value : this->synonymObjects) {
		if (value.getSynonym() == object.getSynonym()) {
			return true;
		}
	}
	return false;
}

bool SynonymTable::contains(SynonymString synonym) {
	for (auto value : this->synonymObjects) {
		if (value.getSynonym().compare(synonym) == 0) {
			return true;
		}
	}
	return false;
}

SynonymObject SynonymTable::getObject(SynonymString synonym) {
	for (auto value : this->synonymObjects) {
		if (value.getSynonym().compare(synonym) == 0) {
			return value;
		}
	}
	return this->invalidObject;
}

SynonymObject SynonymTable::getObject(SynonymValue index) {
	if (index >= this->synonymObjects.size() || index < 0) {
        throw Exception::INVALID_OBJECT_INDEX;
	}

	return this->synonymObjects.at(index);
}

std::vector<SynonymObject> SynonymTable::getObjects()
{
	return this->synonymObjects;
}

SynonymString SynonymTable::toString() {
	SynonymString output = "";
	for (auto value : this->synonymObjects) {
		output.append(value.getTypeString() + " - " + value.getSynonym() + "\n");
	}
	return output;
}

/*
Get the size of table
*/
int SynonymTable::size() {
	return synonymObjects.size();
}
