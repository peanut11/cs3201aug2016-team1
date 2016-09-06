#include "SuchThatArgObject.h"

SuchThatArgObject SuchThatArgObject::createSuchThatArgObject(bool isSynonym, EntityType entityType, std::string stringValue, int integerValue)
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

std::string SuchThatArgObject::getStringValue()
{
	return stringValue;
}

int SuchThatArgObject::getIntegerValue()
{
	return integerValue;
}
