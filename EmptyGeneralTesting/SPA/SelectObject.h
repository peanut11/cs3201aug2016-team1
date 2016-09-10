#pragma once
#include "EntityType.h"
#include "AttrType.h"
#include <string>

class SelectObject {
	
	EntityType entityType;	// to represent what entity it is (procedure, while, assignment...)
	std::string synonymString;	// represent the synonym 
	AttrType::AttrType attrType;	// to represent if it holds a attrType to its entity(procName, varName, ...)
	bool isBoolean;	// to reprsent if its 'Select BOOLEAN' instead

public :
	SelectObject();
	SelectObject(EntityType entityType, std::string synonymString, AttrType::AttrType attrType, bool isBoolean);
	EntityType getEntityType();
	std::string getSynonymString();
	AttrType::AttrType getAttrType();
	bool getBoolean();

};
