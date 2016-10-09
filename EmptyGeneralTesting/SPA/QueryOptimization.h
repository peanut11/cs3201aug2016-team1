#pragma once
#include "ClauseObject.h"
#include "QueryTable.h"
#include "GroupType.h"
#include "SynonymGroup.h"
#include <vector>

class QueryOptimization {

	SynonymGroup mSynonymGroup;

	//std::vector<std::vector<ClauseObject>> groups;
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

	void beginClauseOptimize(std::vector<ClauseObject> clauseObjList);
	
	std::vector<std::vector<ClauseObject>> beginGroup(SynonymGroup mSynonymGroup, QueryTable mQueryTable);
	//std::vector<std::vector<ClauseObject>> getGroups();

	GroupType::GroupType getGroupType(QueryTable mQueryTable, ClauseSuchThatObject mClauseObject);
	GroupType::GroupType getGroupType(QueryTable mQueryTable, ClausePatternObject mClauseObject);
	GroupType::GroupType getGroupType(QueryTable mQueryTable, ClauseWithObject mClauseObject);



	bool isClauseReturnBoolean(ClauseSuchThatObject mClauseObject);
	bool isClauseReturnBoolean(ClausePatternObject mClauseObject);

	bool isClauseAffectsResult(QueryTable mQueryTable, ClauseSuchThatObject mClauseObject);
	bool isClauseAffectsResult(QueryTable mQueryTable, ClausePatternObject mClauseObject);
	bool isClauseAffectsResult(QueryTable mQueryTable, ClauseWithObject mClauseObject);

	bool isClauseOthers(ClauseSuchThatObject mClauseObject);

	

};