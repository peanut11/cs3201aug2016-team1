#pragma once
#include "ClauseObject.h"
#include "QueryTable.h"
#include "GroupType.h"
#include "GroupObject.h"
#include "SynonymGroup.h"
#include <vector>

class QueryOptimization {

	SynonymGroup mSynonymGroup;

	GroupObject sortGroup(GroupObject mGroupObject);

	std::string getRelationshipString(RelationshipType type);
	//std::vector<std::vector<ClauseObject>> groups;

	//std::map<int, GroupObject> mapGroupObject;





public:
	/*
	Assign all ClauseObjects in QueryTable into specific group
	Group 1		: Boolean clauses
	Group 2		: Not related to Result clause
	Group 3		: With clause which contains a constant
	Group 4 - n	: Common synonym between Relationship Clauses, related to Result clause
	Group n + 1	: Next*, Affects, Affects*
	*/

	QueryOptimization();

	std::vector<GroupObject> beginOptimize(SynonymGroup *mSynonymGroup, QueryTable mQueryTable);
	std::map<int, GroupObject> doGroup(SynonymGroup *mSynonymGroup, QueryTable mQueryTable);
	std::vector<GroupObject> doOptimizeGroups(std::map<int, GroupObject> groups);
	
	std::string printFinalResult(std::vector<GroupObject> groups);
	std::string printMapGroupObjectToString(std::map<int, GroupObject> mapGroupObject); //std::map<int, GroupObject> mMap
	
	//std::string getFirstSynonym(ClauseObject* current);
	//std::string getSecondSynonym(ClauseObject* current);

	bool isSynonymGroupAffectResult(QueryTable mQueryTable, std::vector<std::string> synonymList);
	//bool hasSynonymUsedBefore(ClauseObject* current, std::string synonym);
	

	

};