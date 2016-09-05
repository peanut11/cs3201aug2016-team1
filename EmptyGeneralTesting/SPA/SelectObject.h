#pragma once
#include "EntityType.h"
#include "AttrType.h"

class SelectObject {
	
	EntityType entityType;	// to represent what entity it is (procedure, while, assignment...)
	AttrType attrType;	// to represent if it holds a attrType to its entity(procName, varName, ...)
	bool isBoolean;	// to reprsent if its 'Select BOOLEAN' instead

public :

	SelectObject(EntityType entityType, AttrType attrType, bool isBoolean);
	EntityType getEntityType();
	AttrType getAttrType();
	bool getBoolean();

};
