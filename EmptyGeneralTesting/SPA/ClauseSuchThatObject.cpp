#include "ClauseSuchThatObject.h"

ClauseSuchThatObject ClauseSuchThatObject::createSuchThatRelobject(RelationshipType type, ClauseSuchThatArgObject argsOne, ClauseSuchThatArgObject argsTwo)
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

ClauseSuchThatArgObject ClauseSuchThatObject::getArgsOne()
{
	return argsOne;
}

ClauseSuchThatArgObject ClauseSuchThatObject::getArgsTwo()
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
