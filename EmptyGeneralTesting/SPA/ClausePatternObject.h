
#pragma once
#include "EntityType.h"
#include <string>

class ClausePatternObject {

	EntityType patternType;
	EntityType firstArgType;
	std::string firstArg;
	std::string secondArg;
	std::string thirdArg;
	bool isResultsBoolean = false;

public:

	ClausePatternObject();
	ClausePatternObject(EntityType patternType, EntityType firstArgType, std::string firstArg, std::string secondArg);
	ClausePatternObject(EntityType patternType, EntityType firstArgType, std::string firstArg, std::string secondArg, std::string thirdArg);

	EntityType getPatternType();
	EntityType getFirstArgumentType();
	std::string getFirstArgument();
	std::string getSecondArgument();
	std::string getThirdArgument();
	bool getResultsBoolean();
	void setResultsBoolean(bool boolean);
};