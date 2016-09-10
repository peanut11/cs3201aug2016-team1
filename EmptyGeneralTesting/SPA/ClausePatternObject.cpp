
#include "ClausePatternObject.h"

ClausePatternObject::ClausePatternObject() {
}

ClausePatternObject::ClausePatternObject(EntityType patternType, std::string patternSynonymArg, EntityType firstArgType, bool isFirstArgSynonym, std::string firstArg, std::string secondArg) {
	this->patternType = patternType;
	this->patternSynonymArg = patternSynonymArg;
	this->firstArgType = firstArgType;
	this->isFirstArgSynonym = isFirstArgSynonym;
	this->firstArg = firstArg;
	this->secondArg = secondArg;
}

ClausePatternObject::ClausePatternObject(EntityType patternType, std::string patternSynonymArg, EntityType firstArgType, bool isFirstArgSynonym, std::string firstArg, std::string secondArg, std::string thirdArg) {
	this->patternType = patternType;
	this->patternSynonymArg = patternSynonymArg;
	this->firstArgType = firstArgType;
	this->isFirstArgSynonym = isFirstArgSynonym;
	this->firstArg = firstArg;
	this->secondArg = secondArg;
	this->thirdArg = thirdArg;
}


EntityType ClausePatternObject::getPatternType() {
	return this->patternType;
}

EntityType ClausePatternObject::getFirstArgumentType() {
	return this->firstArgType;
}

std::string ClausePatternObject::getPatternSynonymArgument()
{
	return this->patternSynonymArg;
}

std::string ClausePatternObject::getFirstArgument()
{
	return this->firstArg;
}

std::string ClausePatternObject::getSecondArgument()
{
	return this->secondArg;
}

std::string ClausePatternObject::getThirdArgument()
{
	return this->thirdArg;
}

bool ClausePatternObject::getIsFirstArgSynonym()
{
	return this->isFirstArgSynonym;
}

void ClausePatternObject::setResultsBoolean(bool boolean)
{
	this->isResultsBoolean = boolean;
}

bool ClausePatternObject::getResultsBoolean()
{
	return this->isResultsBoolean;
}