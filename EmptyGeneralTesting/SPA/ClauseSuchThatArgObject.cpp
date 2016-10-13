#include "ClauseSuchThatArgObject.h"

int ClauseSuchThatArgObject::EMTPY_INT = 0;

ClauseSuchThatArgObject::ClauseSuchThatArgObject() {}

ClauseSuchThatArgObject::ClauseSuchThatArgObject(EntityType entityType, StringToken stringValue, ProcStmtVarIndex integerValue, bool isSynonym)
{
	this->entityType = entityType;
	this->stringValue = stringValue;
	this->integerValue = integerValue;
	this->isSynonym = isSynonym;
}

bool ClauseSuchThatArgObject::getIsSynonym()
{
	return isSynonym;
}

EntityType ClauseSuchThatArgObject::getEntityType() const
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
