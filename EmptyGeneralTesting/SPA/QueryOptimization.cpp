#include "QueryOptimization.h"


QueryOptimization::QueryOptimization() {
}

/*
Assign all ClauseObjects in QueryTable into specific group
Group 1		: Boolean clauses
Group 2		: Not related to Result clause
Group 3		: With clause which contains a constant
Group 4 - n	: Common synonym between Relationship Clauses, related to Result clause
Group n + 1	: Next*, Affects, Affects*
*/
std::vector<std::vector<ClauseObject>> QueryOptimization::beginGroup(SynonymGroup mSynonymGroup, QueryTable mQueryTable) {
	// First check such that clauses
	// Second check with clauses
	// Third check pattern clauses

	std::map<std::string, int> groupSynonymMap;

	std::vector<std::vector<ClauseObject>> mGroup;
	std::vector<ClauseObject> clausesNoSynonymList; // group 1
	std::vector<ClauseObject> clausesNoRelatedToResultList; // group 2
	std::vector<ClauseObject> withClauseNoRealtedToResultList; // group 3
	std::vector<ClauseObject> clausesRelatedToResultList; // group 4 to n
	std::vector<ClauseObject> otherClausesList; // group n+1

	for (auto value : mQueryTable.getSuchThats()) {
			
	}

	
	//mGroup.push_back(clausesNoSynonymList);
	//mGroup.push_back(clausesNoSynonymList);

	return mGroup;

}

void QueryOptimization::beginClauseOptimize(std::vector<ClauseObject> clauseObjList) {
}
/*
std::vector<std::vector<ClauseObject>> QueryOptimization::getGroups() {
	return this->groups;
}
*/

GroupType::GroupType QueryOptimization::getGroupType(QueryTable mQueryTable, ClauseSuchThatObject mClauseObject) {
	if (!this->isClauseAffectsResult(mQueryTable, mClauseObject)) {
		// does not affect result
		// can be group 1, 2 or others
		if (this->isClauseReturnBoolean(mClauseObject)) {
			return GroupType::GroupType::BOOLEAN;
		}

		return GroupType::GroupType::NOT_RELATED;
	
	}
	else {
		if (this->isClauseOthers(mClauseObject)) {
			return GroupType::GroupType::OTHERS;
		}

		// affect result; group 4 to n
		return GroupType::GroupType::RELATED;
	}
}

GroupType::GroupType QueryOptimization::getGroupType(QueryTable mQueryTable, ClausePatternObject mClauseObject) {
	if (!this->isClauseAffectsResult(mQueryTable, mClauseObject)) {
		// does not affect result
		// can be group 1, 2 or others
		if (this->isClauseReturnBoolean(mClauseObject)) {
			return GroupType::GroupType::BOOLEAN;
		}

		return GroupType::GroupType::NOT_RELATED;

	}
	else {
		// affect result; group 4 to n
		return GroupType::GroupType::RELATED;
	}
}

GroupType::GroupType QueryOptimization::getGroupType(QueryTable mQueryTable, ClauseWithObject mClauseObject) {
	if (!this->isClauseAffectsResult(mQueryTable, mClauseObject)) {
		// does not affect result; group 3
		return GroupType::GroupType::WITH_NOT_RELATED;
	}
	else {
		// affect result; group 4 to n + 1
		return GroupType::GroupType::RELATED;
	}
}


bool QueryOptimization::isClauseReturnBoolean(ClauseSuchThatObject mClauseObject) {
	return ((mClauseObject.getArgsOne().getEntityType() == EntityType::CONSTANT
			|| mClauseObject.getArgsOne().getEntityType() == EntityType::VARIABLE
			|| mClauseObject.getArgsOne().getEntityType() == EntityType::WILDCARD)
		&& (mClauseObject.getArgsTwo().getEntityType() == EntityType::CONSTANT
			|| mClauseObject.getArgsTwo().getEntityType() == EntityType::VARIABLE
		|| mClauseObject.getArgsTwo().getEntityType() == EntityType::WILDCARD));
}

bool QueryOptimization::isClauseReturnBoolean(ClausePatternObject mClauseObject) {
	return (mClauseObject.getFirstArgumentType() == EntityType::WILDCARD
		&& mClauseObject.getSecondArgument().compare("_") == 0)
		&& (mClauseObject.getPatternType() == EntityType::WHILE ? mClauseObject.getThirdArgument().compare("_") == 0 : true);
}




bool QueryOptimization::isClauseAffectsResult(QueryTable mQueryTable, ClauseSuchThatObject mClauseObject) {
	return (mClauseObject.getArgsOne().getIsSynonym()
		&& mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getArgsOne().getStringValue()))
		|| (mClauseObject.getArgsTwo().getIsSynonym()
			&& mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getArgsTwo().getStringValue()));
}

bool QueryOptimization::isClauseAffectsResult(QueryTable mQueryTable, ClausePatternObject mClauseObject) {
	return mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getPatternSynonymArgument())
		|| mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getFirstArgument());
}

bool QueryOptimization::isClauseAffectsResult(QueryTable mQueryTable, ClauseWithObject mClauseObject) {
	return mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getRefObject1().getSynonym()) // may be first arugment is synonym
		|| mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getRefObject2().getSynonym()); // may be second argument is synonym
}

/*
Determine whether the relationship type of the clause is either Next or Affects 
*/
bool QueryOptimization::isClauseOthers(ClauseSuchThatObject mClauseObject) {
	return mClauseObject.getRelationshipType() == RelationshipType::AFFECTS
		|| mClauseObject.getRelationshipType() == RelationshipType::AFFECTS_STAR
		|| mClauseObject.getRelationshipType() == RelationshipType::NEXT
		|| mClauseObject.getRelationshipType() == RelationshipType::NEXT_STAR;
}






