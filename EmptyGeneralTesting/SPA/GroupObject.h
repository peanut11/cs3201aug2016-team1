#pragma once
#include <map>
#include <vector>
#include "GroupType.h"
#include "ClauseObject.h"
#include "ClauseSuchThatObject.h"

class GroupObject {
	GroupType::GroupType type;
	//std::map<int, std::vector<ClauseObject>> mMap;
	std::vector<ClauseObject*> objectList;

public:

	GroupObject();
	//GroupObject(GroupType::GroupType type);
	void insert(ClauseSuchThatObject* object);
	void insert(ClauseObject* object);
	void setGroupType(GroupType::GroupType type);
	std::vector<ClauseObject*> getClauseObjectList();
	GroupType::GroupType getGroupType() const ;
	std::string getGroupTypeString();

	bool operator<(GroupObject obj) {
		return this->getGroupType() < obj.getGroupType();
	}

};