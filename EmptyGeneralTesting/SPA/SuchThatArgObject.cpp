#include "SuchThatArgObject.h"

SuchThatArgObject SuchThatArgObject::createSuchThatArgObject(bool isSynonym, EntityType entityType, StringToken stringValue, ProcStmtVarIndex integerValue)
{
	this->isSynonym = isSynonym;
	this->entityType = entityType;
	this->stringValue = stringValue;
	this->integerValue = integerValue;

	return *this;
}

bool SuchThatArgObject::getIsSynonym()
{
	return isSynonym;
}

EntityType SuchThatArgObject::getEntityType()
{
	return entityType;
}

StringToken SuchThatArgObject::getStringValue()
{
	return stringValue;
}

ProcStmtVarIndex SuchThatArgObject::getIntegerValue()
{
	return integerValue;
}
