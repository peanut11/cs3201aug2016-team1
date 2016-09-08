#include "ClauseSuchThatArgObject.h"

ClauseSuchThatArgObject ClauseSuchThatArgObject::createSuchThatArgObject(bool isSynonym, EntityType entityType, StringToken stringValue, ProcStmtVarIndex integerValue)
{
	this->isSynonym = isSynonym;
	this->entityType = entityType;
	this->stringValue = stringValue;
	this->integerValue = integerValue;

	return *this;
}

bool ClauseSuchThatArgObject::getIsSynonym()
{
	return isSynonym;
}

EntityType ClauseSuchThatArgObject::getEntityType()
{
	return entityType;
}

StringToken ClauseSuchThatArgObject::getStringValue()
{
	return stringValue;
}

ProcStmtVarIndex ClauseSuchThatArgObject::getIntegerValue()
{
	return integerValue;
}
