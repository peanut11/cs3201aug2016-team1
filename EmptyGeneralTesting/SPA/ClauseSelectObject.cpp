#include "ClauseSelectObject.h"
#include "EntityType.h"
#include "AttrType.h"


ClauseSelectObject::ClauseSelectObject()
{
}

ClauseSelectObject::ClauseSelectObject(EntityType entityType, std::string synonymString, AttrType::AttrType attrType, bool isBoolean) {
	ClauseSelectObject(entityType, synonymString, attrType);
}

ClauseSelectObject::ClauseSelectObject(EntityType entityType, std::string synonymString, AttrType::AttrType attrType)
{
	this->entityType = entityType;
	this->synonymString = synonymString;
	this->attrType = attrType;
}

EntityType ClauseSelectObject::getEntityType()
{
	return entityType;
}

std::string ClauseSelectObject::getSynonymString() 
{
	return synonymString;
}

AttrType::AttrType ClauseSelectObject::getAttrType()
{
	return attrType;
}

bool ClauseSelectObject::getBoolean()
{
	return false;
}

void ClauseSelectObject::setEntityType(EntityType entityType) {
	this->entityType = entityType;
}

void ClauseSelectObject::setSynonymString(std::string synonymString) {
	this->synonymString = synonymString;
}

void ClauseSelectObject::setAttrType(AttrType::AttrType attrType) {
	this->attrType = attrType;
}
/*
void ClauseSelectObject::setIsBoolean(bool isBoolean) {
	this->isBoolean = isBoolean;
}
*/

