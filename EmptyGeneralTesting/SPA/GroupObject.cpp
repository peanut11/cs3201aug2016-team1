#include "GroupObject.h"

GroupObject::GroupObject() {
}

/*
GroupObject::GroupObject(GroupType::GroupType type) {
	this->type = type;
}
*/

void GroupObject::insert(ClauseSuchThatObject* object) {
	this->objectList.push_back(object);
}

void GroupObject::insert(ClauseObject* object) {
	this->objectList.push_back(object);
}

void GroupObject::setGroupType(GroupType::GroupType type) {
	this->type = type;
}

GroupType::GroupType GroupObject::getGroupType() const {
	return this->type;
}

std::string GroupObject::getGroupTypeString() {
	switch (this->type) {
	case GroupType::GroupType::BOOLEAN:
		return "BOOLEAN";
	case GroupType::GroupType::NOT_RELATED:
		return "NOT_RELATED";
	case GroupType::GroupType::WITH_NOT_RELATED:
		return "WITH_NOT_RELATED";
	case GroupType::GroupType::RELATED:
		return "RELATED";
	case GroupType::GroupType::OTHERS:
		return "OTHERS";
	}
}

std::vector<ClauseObject*> GroupObject::getClauseObjectList() {
	return this->objectList;
}
