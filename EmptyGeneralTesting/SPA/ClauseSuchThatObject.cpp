#include "ClauseSuchThatObject.h"

ClauseSuchThatObject::ClauseSuchThatObject()
{
	this->mClauseType = ClauseType::ClauseType::SUCH_THAT;
}

ClauseSuchThatObject::ClauseSuchThatObject(RelationshipType type, ClauseSuchThatArgObject* argsOne, ClauseSuchThatArgObject* argsTwo)
{
	this->mClauseType = ClauseType::ClauseType::SUCH_THAT;
	this->type = type;
	this->argsOne = argsOne;
	this->argsTwo = argsTwo;
}

RelationshipType ClauseSuchThatObject::getRelationshipType() const
{
	return type;
}

ClauseSuchThatArgObject* ClauseSuchThatObject::getArgsOne() const
{
	return argsOne;
}

ClauseSuchThatArgObject* ClauseSuchThatObject::getArgsTwo() const
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
