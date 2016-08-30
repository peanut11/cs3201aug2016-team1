
#include "RelTable.h"

RelTable *RelTable::_instance;

/*
Singleton
*/
RelTable *RelTable::getInstance()
{
	if (!_instance)
		_instance = new RelTable;
	return _instance;
}

/*
Insert a new object into RelObjects vector
Return the index of the new object*/
int RelTable::insert(RelObject object) {
	relObjects.push_back(object);
	return relObjects.size() - 1; // last index of the object
}


RelObject RelTable::getObject(int index) {
	if (index >= this->relObjects.size() || index < 0) {
		throw std::runtime_error("getObject index is out of vector size");
	}

	return this->relObjects.at(index);
}


/*
Get the size of table
*/
int RelTable::size() {
	return relObjects.size();
}

