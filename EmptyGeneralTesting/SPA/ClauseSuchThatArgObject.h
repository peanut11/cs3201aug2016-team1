#pragma once

#include <string>

#include "EntityType.h"
#include "Types.h"

class ClauseSuchThatArgObject {

	

	bool isSynonym;
	EntityType entityType;
	StringToken stringValue;
	ProcStmtVarIndex integerValue;

public:

	static int EMTPY_INT;
	
	ClauseSuchThatArgObject();
	ClauseSuchThatArgObject (EntityType entityType, StringToken stringValue, ProcStmtVarIndex integerValue, bool isSynonym);
	
	EntityType getEntityType() const;
	StringToken getStringValue();
	ProcStmtVarIndex getIntegerValue();

	bool getIsSynonym();
};