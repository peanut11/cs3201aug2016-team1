
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



std::set<std::string> SynonymTable::getSetString(std::string synonym) {
	if (synonym.compare("_") == 0) {
		std::set<std::string> fullSet = std::set<std::string>();

		for (auto value : this->synonymObjects) {
			fullSet.insert(value.getSetString().begin(), value.getSetString().end());
		}

		return fullSet;
	}

	if (this->contains(synonym)) {
		return this->getObject(synonym).getSetString();
	}

	return std::set<std::string>();
}

std::set<int> SynonymTable::getSetInt(std::string synonym) {
	if (synonym.compare("_") == 0) {
		std::set<int> fullSet = std::set<int>();

		for (auto value : this->synonymObjects) {
			fullSet.insert(value.getSetInt().begin(), value.getSetInt().end());
		}

		return fullSet;
	}

	if (this->contains(synonym)) {
		return this->getObject(synonym).getSetInt();
	}

	return std::set<int>();
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

bool SynonymTable::replaceSet(std::string synonym, std::set<std::string> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).replaceSet(newSet);
		return true;
	}
	return false;
}

bool SynonymTable::replaceSet(std::string synonym, std::set<int> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).replaceSet(newSet);
		return true;
	}
	return false;
}

bool SynonymTable::insertSet(std::string synonym, std::set<std::string> newSet) {
	if (this->contains(synonym)) {
		this->getObject(synonym).insertSet(newSet);
		return true;
	}
	return false;
}

bool SynonymTable::insertSet(std::string synonym, std::set<int> newSet) {
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

bool SynonymTable::contains(std::string synonym) {
	for (auto value : this->synonymObjects) {
		if (value.getSynonym().compare(synonym) == 0) {
			return true;
		}
	}
	return false;
}

SynonymObject SynonymTable::getObject(std::string synonym) {
	for (auto value : this->synonymObjects) {
		if (value.getSynonym().compare(synonym) == 0) {
			return value;
		}
	}
	return this->invalidObject;
}

SynonymObject SynonymTable::getObject(int index) {
	if (index >= this->synonymObjects.size() || index < 0) {
		throw std::runtime_error("getObject index is out of vector size");
	}

	return this->synonymObjects.at(index);
}

std::string SynonymTable::toString() {
	std::string output = "";
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
