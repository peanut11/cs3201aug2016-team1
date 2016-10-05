#include "QueryOptimization.h"



/*
Assign all ClauseObjects in QueryTable into specific group
Group 1		: Boolean clauses
Group 2		: Not related to Result clause
Group 3		: With clause which contains a constant
Group 4 - n	: Common synonym between Relationship Clauses, related to Result clause
Group n + 1	: Next*, Affects, Affects*
*/
void QueryOptimization::beginClausesGrouping(QueryTable mQueryTable) {
	// First check such that clauses
	// Second check with clauses
	// Third check pattern clauses




}

void QueryOptimization::beginClauseOptimize(std::vector<ClauseObject> clauseObjList) {

}



