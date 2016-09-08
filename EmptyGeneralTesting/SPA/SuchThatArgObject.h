#pragma once

#include <string>

#include "EntityType.h"
#include "Types.h"

class SuchThatArgObject {
	bool isSynonym;
	EntityType entityType;
	StringToken stringValue;
	ProcStmtVarIndex integerValue;

public:
	SuchThatArgObject createSuchThatArgObject(bool isSynonym, EntityType entityType, StringToken stringValue, ProcStmtVarIndex integerValue);
	bool getIsSynonym();
	EntityType getEntityType();
	StringToken getStringValue();
	ProcStmtVarIndex getIntegerValue();
};