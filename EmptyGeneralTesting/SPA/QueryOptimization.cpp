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

	std::vector<ClauseObject> clausesNoSynonymList;
	std::vector<ClauseObject> clausesNoRelatedToResultList;
	std::vector<ClauseObject> withClauseContainsConstantList;
	std::vector<ClauseObject> clausesRelatedToResultList;
	std::vector<ClauseObject> otherClausesList;

	for (auto value : mQueryTable.getSuchThats()) {
		if (value.getArgsOne().getEntityType() == EntityType::CONSTANT 
			|| value.getArgsOne().getEntityType() == EntityType::VARIABLE
			&& value.getArgsTwo().getEntityType() == EntityType::CONSTANT 
			|| value.getArgsTwo().getEntityType() == EntityType::VARIABLE)
		{
			clausesNoSynonymList.push_back(value);
		} 
		else if ((value.getArgsOne().getIsSynonym() 
				&& !mQueryTable.getResult().doesClauseSelectObjectExist(value.getArgsOne().getStringValue()))
			|| (value.getArgsTwo().getIsSynonym() 
				&& !mQueryTable.getResult().doesClauseSelectObjectExist(value.getArgsTwo().getStringValue()))) {

		}

	}



}

void QueryOptimization::beginClauseOptimize(std::vector<ClauseObject> clauseObjList) {

}



