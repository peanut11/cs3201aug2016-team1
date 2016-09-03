#pragma once
#include "EntityType.h"
#include <string>

class PatternTypeObject {
	EntityType entityType;
	std::string leftHand;
	std::string rightHand;

public:
	EntityType getEntityType();
	std::string getLeftHand();
	std::string getRightHand();

};