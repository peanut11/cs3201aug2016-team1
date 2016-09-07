#pragma once
#include <vector>

#include "RelObject.h"

class RelTable {

	static RelTable *_instance;
	static RelObject nullRelObject;
	std::vector<RelObject> relObjects;


public:
	static RelTable *getInstance();

	RelObject getObject(int index);
	RelObject find(RelationshipType type);
	int insert(RelObject object);
	int size();





};
