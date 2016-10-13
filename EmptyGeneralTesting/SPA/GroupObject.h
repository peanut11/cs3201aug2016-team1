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
	bool hasContainAffects = false;

public:

	GroupObject();
	//GroupObject(GroupType::GroupType type);
	void insert(ClauseSuchThatObject* object);
	void insert(ClauseObject* object);
	void setGroupType(GroupType::GroupType type);
	void setHasContainAffects(bool value);

	std::vector<ClauseObject*>& getClauseObjectList();
	GroupType::GroupType getGroupType() const ;
	std::string getGroupTypeString();
	bool getHasContainAffects();

	bool operator<(GroupObject obj) {
		return this->getGroupType() < obj.getGroupType();
	}

};