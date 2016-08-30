
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

/*
Insert a new object into RelObjects vector
Return the index of the new object*/
int SynonymTable::insert(SynonymObject object) {
	synonymObjects.push_back(object);
	return synonymObjects.size() - 1; // last index of the object
}


SynonymObject SynonymTable::getObject(int index) {
	if (index >= this->synonymObjects.size() || index < 0) {
		throw std::runtime_error("getObject index is out of vector size");
	}

	return this->synonymObjects.at(index);
}


/*
Get the size of table
*/
int SynonymTable::size() {
	return synonymObjects.size();
}
