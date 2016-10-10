#pragma once
#include "ClauseObject.h"
#include "QueryTable.h"
#include "GroupType.h"
#include "GroupObject.h"
#include "SynonymGroup.h"
#include <vector>

class QueryOptimization {

	SynonymGroup mSynonymGroup;
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
	//std::vector<std::vector<ClauseObject>> getGroups();

	/*
	GroupType::GroupType getGroupType(QueryTable mQueryTable, ClauseSuchThatObject mClauseObject);
	GroupType::GroupType getGroupType(QueryTable mQueryTable, ClausePatternObject mClauseObject);
	GroupType::GroupType getGroupType(QueryTable mQueryTable, ClauseWithObject mClauseObject);
	bool isClauseReturnBoolean(ClauseSuchThatObject mClauseObject);
	bool isClauseReturnBoolean(ClausePatternObject mClauseObject);
	bool isClauseAffectsResult(QueryTable mQueryTable, ClauseSuchThatObject mClauseObject);
	bool isClauseAffectsResult(QueryTable mQueryTable, ClausePatternObject mClauseObject);
	bool isClauseAffectsResult(QueryTable mQueryTable, ClauseWithObject mClauseObject);
	bool isClauseOthers(ClauseSuchThatObject mClauseObject);
	*/

	bool isSynonymGroupAffectResult(QueryTable mQueryTable, std::vector<std::string> synonymList);

	

	

};