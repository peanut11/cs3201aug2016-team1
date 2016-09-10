
#pragma once
#include "EntityType.h"
#include <string>

class ClausePatternObject {

	EntityType patternType;
	EntityType firstArgType;
	std::string patternSynonymArg;
	std::string firstArg;
	std::string secondArg;
	std::string thirdArg;
	bool isFirstArgSynonym = false;
	bool isResultsBoolean = false;

public:

	ClausePatternObject();
	ClausePatternObject(EntityType patternType, std::string patternSynonymArg, EntityType firstArgType, bool isFirstArgSynonym, std::string firstArg, std::string secondArg);
	ClausePatternObject(EntityType patternType, std::string patternSynonymArg, EntityType firstArgType, bool isFirstArgSynonym, std::string firstArg, std::string secondArg, std::string thirdArg);

	EntityType getPatternType();
	EntityType getFirstArgumentType();
	std::string getPatternSynonymArgument();
	std::string getFirstArgument();
	std::string getSecondArgument();
	std::string getThirdArgument();
	bool getIsFirstArgSynonym();
	bool getResultsBoolean();
	void setResultsBoolean(bool boolean);
};