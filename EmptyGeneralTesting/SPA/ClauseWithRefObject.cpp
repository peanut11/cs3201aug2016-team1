#include "ClauseWithRefObject.h"
#include "AttrType.h"

ClauseWithRefObject::ClauseWithRefObject() {
}

ClauseWithRefObject::ClauseWithRefObject(WithRefType refType, EntityType entityType, std::string synonym, AttrType::AttrType attributeName) {
	this->refType = refType;
	this->entityType = entityType;
	this->synonym = synonym;
	this->attributeName = attributeName;
	this->stringValue = "";
	this->integerValue = -1;
}

ClauseWithRefObject::ClauseWithRefObject(WithRefType refType, EntityType entityType, std::string stringValue) {
	this->refType = refType;
	this->entityType = entityType;
	this->synonym = "";
	this->stringValue = stringValue;
	this->integerValue = -1;
}

ClauseWithRefObject::ClauseWithRefObject(WithRefType refType, EntityType entityType, int integerValue) {
	this->refType = refType;
	this->entityType = entityType;
	this->integerValue = integerValue;
	this->synonym = "";
	this->stringValue = "";
}

WithRefType ClauseWithRefObject::getRefType() {
	return refType;
}

EntityType ClauseWithRefObject::getEntityType() {
	return entityType;
}

std::string ClauseWithRefObject::getSynonym() {
	return synonym;
}

std::string ClauseWithRefObject::getStringValue() {
	return stringValue;
}

AttrType::AttrType ClauseWithRefObject::getAttrType() {
	return attributeName;
}

int ClauseWithRefObject::getIntegerValue() {
	return integerValue;
}
