#pragma once
#include "EntityType.h"
#include <string>

class SuchThatArgObject {
	bool isSynonym;
	EntityType entityType;
	std::string stringValue;
	int integerValue;

public:
	SuchThatArgObject createSuchThatArgObject(bool isSynonym, EntityType entityType, std::string stringValue, int integerValue);
	bool getIsSynonym();
	EntityType getEntityType();
	std::string getStringValue();
	int getIntegerValue();

};