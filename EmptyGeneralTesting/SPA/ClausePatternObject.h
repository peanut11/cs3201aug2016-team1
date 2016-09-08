
#pragma once
#include "EntityType.h"
#include <string>

class ClausePatternObject {

	EntityType entityType;
	std::string leftHand;
	std::string rightHand;

public:

	ClausePatternObject(EntityType entityType, std::string leftHand, std::string rightHand);

	EntityType getEntityType();
	std::string getLeftHand();
	std::string getRightHand();

};