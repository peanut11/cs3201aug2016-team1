#include "WithRefObject.h"

WithRefObject WithRefObject::CreateWithRefObject(WithRefType refType, std::string synonym, AttrType attributeName)
{
	this->refType = refType;
	this->synonym = synonym;
	this->attributeName = attributeName;
}

WithRefObject WithRefObject::CreateWithRefObject(WithRefType refType, std::string synonym)
{
	this->refType = refType;
	this->synonym = synonym;
}

WithRefObject WithRefObject::CreateWithRefObject(WithRefType refType, int integerValue)
{
	this->refType = refType;
	this->integerValue = integerValue;
}

WithRefType WithRefObject::getRefType()
{
	return refType;
}

EntityType WithRefObject::getEntityType()
{
	return entityType;
}

std::string WithRefObject::getSynonym()
{
	return synonym;
}

AttrType WithRefObject::getAttrType()
{
	return attributeName;
}

int WithRefObject::getIntegerValue()
{
	return integerValue;
}
