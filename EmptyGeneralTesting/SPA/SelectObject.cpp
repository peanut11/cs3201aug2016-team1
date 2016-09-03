#include "SelectObject.h"
#include "EntityType.h"
#include "AttrType.h"


SelectObject::SelectObject(EntityType entityType, AttrType attrType, bool isBoolean)
{
	this->entityType = entityType;
	this->attrType = attrType;
	this->isBoolean = isBoolean;
}

EntityType SelectObject::getEntityType()
{
	return entityType;
}

AttrType SelectObject::getAttrType()
{
	return attrType;
}

bool SelectObject::getBoolean()
{
	return isBoolean;
}

