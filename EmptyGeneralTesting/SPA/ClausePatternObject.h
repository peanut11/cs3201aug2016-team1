
#pragma once
#include "EntityType.h"
#include <string>

class ClausePatternObject {

	EntityType entityType;
	std::string leftHand;
	std::string rightHand;
	bool isResultsBoolean = false;

public:

	ClausePatternObject();
	ClausePatternObject(EntityType entityType, std::string leftHand, std::string rightHand);

	EntityType getEntityType();
	std::string getLeftHand();
	std::string getRightHand();
	bool getResultsBoolean();
	void setResultsBoolean(bool boolean);
};