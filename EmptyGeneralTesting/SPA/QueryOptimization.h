#pragma once
#include "ClauseObject.h"
#include "QueryTable.h"
#include <vector>

class QueryOptimization {


	std::vector<std::vector<ClauseObject>> groups;

public:
	/*
	Assign all ClauseObjects in QueryTable into specific group
	Group 1		: Boolean clauses
	Group 2		: Not related to Result clause
	Group 3		: With clause which contains a constant
	Group 4 - n	: Common synonym between Relationship Clauses, related to Result clause
	Group n + 1	: Next*, Affects, Affects*
	*/
	void beginClausesGrouping(QueryTable mQueryTable);
	void beginClauseOptimize(std::vector<ClauseObject> clauseObjList);

};