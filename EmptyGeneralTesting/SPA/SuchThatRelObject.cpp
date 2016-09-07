#include "SuchThatRelObject.h"

SuchThatRelObject SuchThatRelObject::createSuchThatRelobject(RelationshipType type, SuchThatArgObject argsOne, SuchThatArgObject argsTwo)
{
	this->type = type;
	this->argsOne = argsOne;
	this->argsTwo = argsTwo;
	return *this;
}

RelationshipType SuchThatRelObject::getRelationshipType()
{
	return type;
}

SuchThatArgObject SuchThatRelObject::getArgsOne()
{
	return argsOne;
}

SuchThatArgObject SuchThatRelObject::getArgsTwo()
{
	return argsTwo;
}

void SuchThatRelObject::setResultsBoolean(bool boolean)
{
	this->isResultsBoolean = boolean;
}

bool SuchThatRelObject::getResultsBoolean()
{
	return this->isResultsBoolean;
}
