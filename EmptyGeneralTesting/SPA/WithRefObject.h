#pragma once
#include "AttrType.h"
#include "WithRefType.h"
#include "EntityType.h"
#include <string>

class WithRefObject {

	WithRefType refType;
	EntityType entityType;
	std::string synonym;
	AttrType::AttrType attributeName;
	int integerValue;

public:
	// the ref-object contains a synonym and attribute (e.g s.stmt#, p.procName)
	WithRefObject CreateWithRefObject(WithRefType refType, std::string synonym, AttrType::AttrType attributeName); 
	// the ref-object is an identifier (e.g if WithRefType = Synonym, the string 'synonym' will contain the synonym)
	WithRefObject CreateWithRefObject(WithRefType refType, std::string synonym);
	// the ref-object is an integer (e.g WithRefType = INTEGER, the int 'integerValue' will contain the value)
	WithRefObject CreateWithRefObject(WithRefType refType, int integerValue);
	WithRefType getRefType();
	EntityType getEntityType();
	std::string getSynonym();
	AttrType::AttrType getAttrType();
	int getIntegerValue();

};