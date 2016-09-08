#include "ClauseSuchThatObject.h"

ClauseSuchThatObject ClauseSuchThatObject::createSuchThatRelobject(RelationshipType type, SuchThatArgObject argsOne, SuchThatArgObject argsTwo)
{
	this->type = type;
	this->argsOne = argsOne;
	this->argsTwo = argsTwo;
	return *this;
}

RelationshipType ClauseSuchThatObject::getRelationshipType()
{
	return type;
}

SuchThatArgObject ClauseSuchThatObject::getArgsOne()
{
	return argsOne;
}

SuchThatArgObject ClauseSuchThatObject::getArgsTwo()
{
	return argsTwo;
}

void ClauseSuchThatObject::setResultsBoolean(bool boolean)
{
	this->isResultsBoolean = boolean;
}

bool ClauseSuchThatObject::getResultsBoolean()
{
	return this->isResultsBoolean;
}
