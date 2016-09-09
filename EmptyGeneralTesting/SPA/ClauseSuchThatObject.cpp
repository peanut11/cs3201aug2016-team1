#include "ClauseSuchThatObject.h"

ClauseSuchThatObject::ClauseSuchThatObject()
{
}

ClauseSuchThatObject::ClauseSuchThatObject(RelationshipType type, ClauseSuchThatArgObject argsOne, ClauseSuchThatArgObject argsTwo)
{
	this->type = type;
	this->argsOne = argsOne;
	this->argsTwo = argsTwo;
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
