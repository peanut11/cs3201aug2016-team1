#include "WithRefObject.h"
#include "AttrType.h"

WithRefObject WithRefObject::CreateWithRefObject(WithRefType refType, std::string synonym, AttrType::AttrType attributeName) {
	this->refType = refType;
	this->synonym = synonym;
	this->attributeName = attributeName;

	return *this;
}

WithRefObject WithRefObject::CreateWithRefObject(WithRefType refType, std::string synonym) {
	this->refType = refType;
	this->synonym = synonym;

	return *this;
}

WithRefObject WithRefObject::CreateWithRefObject(WithRefType refType, int integerValue) {
	this->refType = refType;
	this->integerValue = integerValue;

	return *this;
}

WithRefType WithRefObject::getRefType() {
	return refType;
}

EntityType WithRefObject::getEntityType() {
	return entityType;
}

std::string WithRefObject::getSynonym() {
	return synonym;
}

AttrType WithRefObject::getAttrType() {
	return attributeName;
}

int WithRefObject::getIntegerValue() {
	return integerValue;
}
