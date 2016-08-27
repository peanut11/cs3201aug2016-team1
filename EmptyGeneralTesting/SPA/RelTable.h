#pragma once
#include <vector>

#include "RelObject.h"

class RelTable {

	static RelTable *_instance;
	std::vector<RelObject> relObjects;


public:
	static RelTable *getInstance();

	RelObject getObject(int index);
	int insert(RelObject object);
	int size();





};
