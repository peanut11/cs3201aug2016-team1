#pragma once
#include "ClauseObject.h"
#include "EntityType.h"
#include "AttrType.h"
#include <string>

class ClauseSelectObject {
	
	EntityType entityType;	// to represent what entity it is (procedure, while, assignment...)
	std::string synonymString;	// represent the synonym 
	AttrType::AttrType attrType;	// to represent if it holds a attrType to its entity(procName, varName, ...)
	//bool isBoolean;	// to reprsent if its 'Select BOOLEAN' instead

public :
	ClauseSelectObject();
	ClauseSelectObject(EntityType entityType, std::string synonymString, AttrType::AttrType attrType, bool isBoolean);
	ClauseSelectObject(EntityType entityType, std::string synonymString, AttrType::AttrType attrType);
	EntityType getEntityType();
	std::string getSynonymString();
	AttrType::AttrType getAttrType();
	bool getBoolean();

	void setEntityType(EntityType entityType);
	void setSynonymString(std::string synonymString);
	void setAttrType(AttrType::AttrType attrType);
	//void setIsBoolean(bool isBoolean);


};
