#include "ClauseWithRefObject.h"
#include "AttrType.h"

ClauseWithRefObject::ClauseWithRefObject() {
}

ClauseWithRefObject::ClauseWithRefObject(WithRefType refType, std::string synonym, AttrType::AttrType attributeName) {
	this->refType = refType;
	this->synonym = synonym;
	this->attributeName = attributeName;
	this->stringValue = "";
	this->integerValue = -1;
}

ClauseWithRefObject::ClauseWithRefObject(WithRefType refType, std::string stringValue) {
	this->refType = refType;
	this->synonym = "";
	this->stringValue = stringValue;
	this->integerValue = -1;
}

ClauseWithRefObject::ClauseWithRefObject(WithRefType refType, int integerValue) {
	this->refType = refType;
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
