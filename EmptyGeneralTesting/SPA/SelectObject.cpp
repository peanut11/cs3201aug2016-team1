#include "SelectObject.h"
#include "EntityType.h"
#include "AttrType.h"


SelectObject::SelectObject()
{
}

SelectObject::SelectObject(EntityType entityType, std::string synonymString, AttrType::AttrType attrType, bool isBoolean)
{
	this->entityType = entityType;
	this->synonymString = synonymString;
	this->attrType = attrType;
	this->isBoolean = isBoolean;
}

EntityType SelectObject::getEntityType()
{
	return entityType;
}

std::string SelectObject::getSynonymString() 
{
	return synonymString;
}

AttrType::AttrType SelectObject::getAttrType()
{
	return attrType;
}

bool SelectObject::getBoolean()
{
	return isBoolean;
}

void SelectObject::setEntityType(EntityType entityType) {
	this->entityType = entityType;
}

void SelectObject::setSynonymString(std::string synonymString) {
	this->synonymString = synonymString;
}

void SelectObject::setAttrType(AttrType::AttrType attrType) {
	this->attrType = attrType;
}

void SelectObject::setIsBoolean(bool isBoolean) {
	this->isBoolean = isBoolean;
}


