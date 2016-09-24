#pragma once
#include "AttrType.h"
#include "WithRefType.h"
#include "EntityType.h"
#include <string>

class ClauseWithRefObject {

	WithRefType refType;
	EntityType entityType;
	AttrType::AttrType attributeName;
	std::string synonym;
	std::string stringValue;
	int integerValue;

public:

	ClauseWithRefObject();
	// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
	ClauseWithRefObject(WithRefType refType, std::string synonym, AttrType::AttrType attributeName); 
	// the ref-object is an identifier (e.g if WithRefType = Synonym, the string 'synonym' will contain the synonym)
	ClauseWithRefObject(WithRefType refType, std::string stringValue);
	// the ref-object is an integer (e.g WithRefType = INTEGER, the int 'integerValue' will contain the value)
	ClauseWithRefObject(WithRefType refType, int integerValue);

	WithRefType getRefType();
	EntityType getEntityType();
	std::string getSynonym();
	std::string getStringValue();
	AttrType::AttrType getAttrType();
	int getIntegerValue();

	

};