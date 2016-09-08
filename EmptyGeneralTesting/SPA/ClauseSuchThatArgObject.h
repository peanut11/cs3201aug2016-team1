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
	ClauseSuchThatArgObject createSuchThatArgObject(bool isSynonym, EntityType entityType, StringToken stringValue, ProcStmtVarIndex integerValue);
	bool getIsSynonym();
	EntityType getEntityType();
	StringToken getStringValue();
	ProcStmtVarIndex getIntegerValue();
};