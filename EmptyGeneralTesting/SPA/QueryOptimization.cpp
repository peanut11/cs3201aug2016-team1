#include <algorithm>
#include <vector>
#include "QueryOptimization.h"
#include "GroupObject.h"

QueryOptimization::QueryOptimization() {
}

struct sort_recomputed_synonym
{
	bool operator() (ClauseObject* lhs, ClauseObject* rhs) {
		if (hasSynonymUsedBefore(lhs, getFirstSynonym(rhs)) || hasSynonymUsedBefore(lhs, getSecondSynonym(rhs))) {
			return true;
		}

		return false;
	}

	std::string getFirstSynonym(ClauseObject* current) {

		switch (current->getClauseType()) {

		case ClauseType::ClauseType::PATTERN: {
			ClausePatternObject* derivedPattern = dynamic_cast<ClausePatternObject*>(current);
			return derivedPattern->getPatternSynonymArgument();

		}
		case ClauseType::ClauseType::WITH: {
			ClauseWithObject* derivedWith = dynamic_cast<ClauseWithObject*>(current);
			return derivedWith->getRefObject1()->getSynonym();
		}
		case ClauseType::ClauseType::SUCH_THAT: {
			ClauseSuchThatObject* derivedSuchThat = dynamic_cast<ClauseSuchThatObject*>(current);
			if (derivedSuchThat->getArgsOne()->getIsSynonym()) {
				return derivedSuchThat->getArgsOne()->getStringValue();
			}
			else {
				return "";
			}
			//break;
		}

		}

		return "";
	}

	std::string getSecondSynonym(ClauseObject* current) {
		switch (current->getClauseType()) {

		case ClauseType::ClauseType::PATTERN: {
			// Pattern second arg is always an expression
			ClausePatternObject* derivedPattern = dynamic_cast<ClausePatternObject*>(current);
			if (derivedPattern->getIsFirstArgSynonym()) {
				return derivedPattern->getFirstArgument();
			}
			return "";

		}
		case ClauseType::ClauseType::WITH: {
			ClauseWithObject* derivedWith = dynamic_cast<ClauseWithObject*>(current);
			return derivedWith->getRefObject2()->getSynonym();
		}
		case ClauseType::ClauseType::SUCH_THAT: {
			ClauseSuchThatObject* derivedSuchThat = dynamic_cast<ClauseSuchThatObject*>(current);
			if (derivedSuchThat->getArgsTwo()->getIsSynonym()) {
				return derivedSuchThat->getArgsTwo()->getStringValue();
			}
			else {
				return "";
			}
		}
		}
		return "";

	}

	bool hasSynonymUsedBefore(ClauseObject* current, std::string synonym) {

		switch (current->getClauseType()) {

		case ClauseType::ClauseType::PATTERN: {
			ClausePatternObject* derivedPattern = dynamic_cast<ClausePatternObject*>(current);
			if (derivedPattern->getIsFirstArgSynonym()) {
				return derivedPattern->getFirstArgument().compare(synonym) == 0;
			}
			else {
				return false;
			}

		}
		case ClauseType::ClauseType::WITH: {
			ClauseWithObject* derivedWith = dynamic_cast<ClauseWithObject*>(current);

			return derivedWith->getRefObject1()->getSynonym().compare(synonym) == 0 ||
				derivedWith->getRefObject2()->getSynonym().compare(synonym) == 0;

		}
		case ClauseType::ClauseType::SUCH_THAT: {
			ClauseSuchThatObject* derivedSuchThat = dynamic_cast<ClauseSuchThatObject*>(current);

			if (derivedSuchThat->getArgsOne()->getIsSynonym()) {
				return derivedSuchThat->getArgsOne()->getStringValue().compare(synonym) == 0;
			}

			if (derivedSuchThat->getArgsTwo()->getIsSynonym()) {
				return derivedSuchThat->getArgsTwo()->getStringValue().compare(synonym) == 0;
			}

			return false;

		}
		}

		return false;

	}

};

struct sort_clause_type
{
	bool operator() (ClauseObject* lhs, ClauseObject* rhs) {
		if (lhs->getClauseType() == ClauseType::ClauseType::WITH) {
			return true;
		}

		if (rhs->getClauseType() == ClauseType::ClauseType::WITH) {
			return false;
		}
		/*
		if (lhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT &&
			rhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT) {
			return false;
		}
		return lhs->getClauseType() < rhs->getClauseType();
		*/
		return false;
	}

};

struct sort_relationship_type {
	bool operator() (ClauseObject* lhs, ClauseObject* rhs) {

		if (lhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT &&
			rhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT) {

			ClauseSuchThatObject* lhsObj = dynamic_cast<ClauseSuchThatObject*>(lhs); // const_cast< const ClauseSuchThatObject*>(lhs)
			ClauseSuchThatObject* rhsObj = dynamic_cast<ClauseSuchThatObject*>(rhs);

			if ((lhsObj->getRelationshipType() == MODIFIES || lhsObj->getRelationshipType() == MODIFIES_P)
				&& (rhsObj->getRelationshipType() == USES || rhsObj->getRelationshipType() == USES_P)) {
				return true;
			}

			if (lhsObj->getRelationshipType() == USES && rhsObj->getRelationshipType() == USES_P) {
				return true;
			}

			if (lhsObj->getRelationshipType() == USES_P && rhsObj->getRelationshipType() == USES) {
				return false;
			}

		}
		
		return false;
	}
};

struct sort_number_of_synonym {
	bool operator() (ClauseObject* lhs, ClauseObject* rhs) {
	
		if (lhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT &&
			rhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT) {

			ClauseSuchThatObject* lhsObj = dynamic_cast<ClauseSuchThatObject*>(lhs); // const_cast< const ClauseSuchThatObject*>(lhs)
			ClauseSuchThatObject* rhsObj = dynamic_cast<ClauseSuchThatObject*>(rhs);

			return lhsObj->getNumberOfSynonym() < rhsObj->getNumberOfSynonym();
		}
		return false;
	}
};

/*
Assign all ClauseObjects in QueryTable into specific group
Group 1		: Boolean clauses
Group 2		: Not related to Result clause
Group 3		: With clause which contains a constant
Group 4 - n	: Common synonym between Relationship Clauses, related to Result clause
Group n + 1	: Next*, Affects, Affects*
*/
std::vector<GroupObject> QueryOptimization::beginOptimize(SynonymGroup *mSynonymGroup, QueryTable mQueryTable) {

	std::map<int, GroupObject> groups = this->doGroup(mSynonymGroup, mQueryTable);
	std::vector<GroupObject> finalResult = this->doOptimizeGroups(groups);

	return finalResult;

}


std::map<int, GroupObject> QueryOptimization::doGroup(SynonymGroup *mSynonymGroup, QueryTable mQueryTable) {

	std::map<int, GroupObject> mapGroupObject;

	for (ClauseSuchThatObject* value : mQueryTable.getSuchThats()) {

		int index = 0;

		if (value->getArgsOne()->getIsSynonym()) { // if arg1 is synonym
			index = mSynonymGroup->getGroupIndex(value->getArgsOne()->getStringValue());
		}
		else if (value->getArgsTwo()->getIsSynonym()) { // else if arg2 is synonym
			index = mSynonymGroup->getGroupIndex(value->getArgsTwo()->getStringValue());
		}

		std::map<int, GroupObject>::iterator iter = mapGroupObject.find(index);

		if (iter != mapGroupObject.end()) {
			mapGroupObject[index].insert(value->clone());
		}
		else {
			GroupObject newObject = GroupObject();
			newObject.insert(value->clone());
			mapGroupObject[index] = newObject;

			if (index == 0) {
				mapGroupObject[index].setGroupType(GroupType::GroupType::BOOLEAN);
			}

		}

		if (value->getRelationshipType() == RelationshipType::AFFECTS
			|| value->getRelationshipType() == RelationshipType::AFFECTS_STAR) {
			mapGroupObject[index].setHasContainAffects(true);
		}


	}

	for (ClauseWithObject* value : mQueryTable.getWiths()) {
		int index = 0;

		if (value->getRefObject1()->getRefType() == WithRefType::SYNONYM || value->getRefObject1()->getRefType() == WithRefType::ATTRREF) {
			index = mSynonymGroup->getGroupIndex(value->getRefObject1()->getSynonym());
		}
		else if (value->getRefObject2()->getRefType() == WithRefType::SYNONYM) {
			index = mSynonymGroup->getGroupIndex(value->getRefObject2()->getSynonym());
		}

		std::map<int, GroupObject>::iterator iter = mapGroupObject.find(index);

		if (iter != mapGroupObject.end()) {
			mapGroupObject[index].insert(value->clone());
		}
		else {
			GroupObject newObject = GroupObject();
			newObject.insert(value->clone());
			mapGroupObject[index] = newObject;

			if (index == 0) {
				mapGroupObject[index].setGroupType(GroupType::GroupType::BOOLEAN);
			}

		}

	}

	for (ClausePatternObject* value : mQueryTable.getPatterns()) {
		int index = 0;

		index = mSynonymGroup->getGroupIndex(value->getPatternSynonymArgument());

		if (index == -1) {
			// the synonym is not related 
			// try check first argument synonym
			if (value->getIsFirstArgSynonym()) {
				index = mSynonymGroup->getGroupIndex(value->getFirstArgument());
			}
		}

		std::map<int, GroupObject>::iterator iter = mapGroupObject.find(index);

		if (iter != mapGroupObject.end()) {
			mapGroupObject[index].insert(value->clone());
		}
		else {
			GroupObject newObject = GroupObject();
			newObject.insert(value->clone());
			mapGroupObject[index] = newObject;

			if (index == 0 || index == -1) {
				mapGroupObject[index].setGroupType(GroupType::GroupType::BOOLEAN);
			}

		}

	}




	for (std::map<int, GroupObject>::iterator ii = mapGroupObject.begin(); ii != mapGroupObject.end(); ii++) {
		int index = ii->first;

		if (index == 0) { continue; } // confirm is not related and BOOLEAN

		std::vector <std::string> synonyms = mSynonymGroup->getSynonyms(index);

		if (this->isSynonymGroupAffectResult(mQueryTable, synonyms)) {
			mapGroupObject[index].setGroupType(GroupType::GroupType::RELATED);
		}
		else {
			if (mapGroupObject[index].getHasContainAffects()) {
				mapGroupObject[index].setGroupType(GroupType::GroupType::NOT_RELATED_CONTAIN_AFFECTS);
			}
			else {
				mapGroupObject[index].setGroupType(GroupType::GroupType::NOT_RELATED);
			}
			
		}

	}

	
	return mapGroupObject;
	//return true;

}

std::vector<GroupObject> QueryOptimization::doOptimizeGroups(std::map<int, GroupObject> groups) {
	std::vector<GroupObject> result;

	for (std::map<int, GroupObject>::iterator ii = groups.begin(); ii != groups.end(); ii++) {		
		result.push_back(this->sortGroup(ii->second)); // sort
		//result.push_back(ii->second); // not sort
	}
	
	sort(result.begin(), result.end(), [](const GroupObject& lhs, const GroupObject& rhs)
	{
		// if true, means lhs is before rhs
		// if false, means lhs is after rhs
		return lhs.getGroupType() < rhs.getGroupType();
	});
	
	return result;

}

GroupObject QueryOptimization::sortGroup(GroupObject mGroupObject) {

	/*
	1. Prioritize clauses with one constant and one synonym
	2. Prioritize clauses with less number of results: Follows, Modifies, etc.
	3. Sort clauses such that at least one synonym has been computed in a previous clause
	4. Prioritize with-clauses – more restrictive than such that clauses
	5. Evaluating pattern-clauses – similar to any such that clause
	6. Push Affects/* clauses on the last positions in a group
	*/
	
	//sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(), sort_recomputed_synonym());
	sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(), sort_clause_type());
	sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(), sort_relationship_type());
	sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(), sort_number_of_synonym());

	/*
	sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(), 
		[this](ClauseObject* lhs, ClauseObject* rhs) {
		
		if (hasSynonymUsedBefore(lhs, getFirstSynonym(rhs)) || hasSynonymUsedBefore(lhs, getSecondSynonym(rhs))) {
			return true;
		}

		return false;

	});
	
	// Sort Clause Type
	sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(),
		[this](ClauseObject* lhs, ClauseObject* rhs) {

		if (lhs->getClauseType() == ClauseType::ClauseType::WITH) {
			return true;
		}

		if (rhs->getClauseType() == ClauseType::ClauseType::WITH) {
			return false;
		}

		return false;
	});

	// Sort Relationship Type
	sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(),
		[this](ClauseObject* lhs, ClauseObject* rhs) {

		if (lhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT &&
			rhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT) {

			ClauseSuchThatObject* lhsObj = dynamic_cast<ClauseSuchThatObject*>(lhs); // const_cast< const ClauseSuchThatObject*>(lhs)
			ClauseSuchThatObject* rhsObj = dynamic_cast<ClauseSuchThatObject*>(rhs);

			if ((lhsObj->getRelationshipType() == MODIFIES || lhsObj->getRelationshipType() == MODIFIES_P)
				&& (rhsObj->getRelationshipType() == USES || rhsObj->getRelationshipType() == USES_P)) {
				return true;
			}

			if (lhsObj->getRelationshipType() == USES && rhsObj->getRelationshipType() == USES_P) {
				return true;
			}

			if (lhsObj->getRelationshipType() == USES_P && rhsObj->getRelationshipType() == USES) {
				return false;
			}

		}

		return false;
	});

	// Sort Number of Synonyms
	sort(mGroupObject.getClauseObjectList().begin(), mGroupObject.getClauseObjectList().end(),
		[this](ClauseObject* lhs, ClauseObject* rhs) {

		if (lhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT &&
			rhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT) {

			ClauseSuchThatObject* lhsObj = dynamic_cast<ClauseSuchThatObject*>(lhs); // const_cast< const ClauseSuchThatObject*>(lhs)
			ClauseSuchThatObject* rhsObj = dynamic_cast<ClauseSuchThatObject*>(rhs);

			return lhsObj->getNumberOfSynonym() < rhsObj->getNumberOfSynonym();
		}
		return false;
	});
	*/
	
	return mGroupObject;

}


std::string QueryOptimization::printFinalResult(std::vector<GroupObject> groups) {
	std::string output = "";
	output.append("============ FINAL RESULT ===============\n");
	for (GroupObject groupObject : groups) {

		output.append("+++\n");
		output.append("(" + groupObject.getGroupTypeString() + ") :");

		for (ClauseObject* value : groupObject.getClauseObjectList()) {
			switch (value->getClauseType()) {
			case ClauseType::ClauseType::SUCH_THAT:
			{
				ClauseSuchThatObject *obj = dynamic_cast<ClauseSuchThatObject*>(value);
				output.append("\t" + this->getRelationshipString(obj->getRelationshipType()) + "(" + obj->getArgsOne()->getStringValue() + ", " + obj->getArgsTwo()->getStringValue() + ")"); // this->getRelationshipString(obj->getRelationshipType())
				break;
			}
			case ClauseType::ClauseType::PATTERN:
				output.append("\tpattern");
				break;

			case ClauseType::ClauseType::WITH:
				output.append("\t with");
				break;

			}
		}
		output.append("\n");
	}

	return output;
}

std::string QueryOptimization::printMapGroupObjectToString(std::map<int, GroupObject> mapGroupObject) { // std::map<int, GroupObject> mMap
	std::string output = "";
	output.append("============ OPTIMIZE GROUP RESULT ===============\n");
	for (std::map<int, GroupObject>::iterator ii = mapGroupObject.begin(); ii != mapGroupObject.end(); ii++) {
	//for (auto& kv : mMap) {

		output.append("+++\n");
		output.append(std::to_string(ii->first) + " (" + ii->second.getGroupTypeString() + ") :");
		for (ClauseObject* value : ii->second.getClauseObjectList()) {
			
			switch (value->getClauseType()) {
			case ClauseType::ClauseType::SUCH_THAT:
			{
				//ClauseSuchThatObject *obj = (ClauseSuchThatObject *) &value;
				ClauseSuchThatObject *obj = dynamic_cast<ClauseSuchThatObject*>(value);
				output.append("\t" + this->getRelationshipString(obj->getRelationshipType()) + "(" + obj->getArgsOne()->getStringValue() + ", " + obj->getArgsTwo()->getStringValue() + ")"); // this->getRelationshipString(obj->getRelationshipType())
				
				//output.append("\tsuchthat");
				
				break;
			}
			case ClauseType::ClauseType::PATTERN:
				output.append("\tpattern");
				break;

			case ClauseType::ClauseType::WITH:
				output.append("\twith");
				break;

			}
			//output.append("\n");
		
		}

		output.append("\n");
	}

	return output;
}


/*
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
	return ((mClauseObject.getArgsOne()->getEntityType() == EntityType::CONSTANT
			|| mClauseObject.getArgsOne()->getEntityType() == EntityType::VARIABLE
			|| mClauseObject.getArgsOne()->getEntityType() == EntityType::WILDCARD)
		&& (mClauseObject.getArgsTwo()->getEntityType() == EntityType::CONSTANT
			|| mClauseObject.getArgsTwo()->getEntityType() == EntityType::VARIABLE
		|| mClauseObject.getArgsTwo()->getEntityType() == EntityType::WILDCARD));
}

bool QueryOptimization::isClauseReturnBoolean(ClausePatternObject mClauseObject) {
	return (mClauseObject.getFirstArgumentType() == EntityType::WILDCARD
		&& mClauseObject.getSecondArgument().compare("_") == 0)
		&& (mClauseObject.getPatternType() == EntityType::WHILE ? mClauseObject.getThirdArgument().compare("_") == 0 : true);
}

bool QueryOptimization::isClauseAffectsResult(QueryTable mQueryTable, ClauseSuchThatObject mClauseObject) {
	return (mClauseObject.getArgsOne()->getIsSynonym()
		&& mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getArgsOne()->getStringValue()))
		|| (mClauseObject.getArgsTwo()->getIsSynonym()
			&& mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getArgsTwo()->getStringValue()));
}

bool QueryOptimization::isClauseAffectsResult(QueryTable mQueryTable, ClausePatternObject mClauseObject) {
	return mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getPatternSynonymArgument())
		|| mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getFirstArgument());
}

bool QueryOptimization::isClauseAffectsResult(QueryTable mQueryTable, ClauseWithObject mClauseObject) {
	return mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getRefObject1().getSynonym()) // may be first arugment is synonym
		|| mQueryTable.getResult().doesClauseSelectObjectExist(mClauseObject.getRefObject2().getSynonym()); // may be second argument is synonym
}

bool QueryOptimization::isClauseOthers(ClauseSuchThatObject mClauseObject) {
	return mClauseObject.getRelationshipType() == RelationshipType::AFFECTS
		|| mClauseObject.getRelationshipType() == RelationshipType::AFFECTS_STAR
		|| mClauseObject.getRelationshipType() == RelationshipType::NEXT
		|| mClauseObject.getRelationshipType() == RelationshipType::NEXT_STAR;
}
*/




bool QueryOptimization::isSynonymGroupAffectResult(QueryTable mQueryTable, std::vector<std::string> synonymList) {
	for (auto synonym : synonymList) {
		if (mQueryTable.getResult().doesClauseSelectObjectExist(synonym)) {
			return true;
		}
	}
	return false;
}





/*
Determine whether the relationship type of the clause is either Next or Affects 
*/


std::string QueryOptimization::getRelationshipString(RelationshipType type) {
	switch (type) {
	case MODIFIES:
		return "modifies";
	case MODIFIES_P:
		return "modifies_p";
	case USES:
		return "uses";
	case USES_P:
		return "uses_p";
	case CALLS:
		return "calls";
	case CALLS_STAR:
		return "calls*";
	case FOLLOWS:
		return "follows";
	case FOLLOWS_STAR:
		return "follows*";
	case PARENT:
		return "parent";
	case PARENT_STAR:
		return "parent*";
	case NEXT:
		return "next";
	case NEXT_STAR:
		return "next*";
	case AFFECTS:
		return "affects";
	case AFFECTS_STAR:
		return "affects*";
	default:
		return "";
	}
}

std::string QueryOptimization::getFirstSynonym(ClauseObject* current) {

	switch (current->getClauseType()) {

	case ClauseType::ClauseType::PATTERN: {
		ClausePatternObject* derivedPattern = dynamic_cast<ClausePatternObject*>(current);
		return derivedPattern->getPatternSynonymArgument();

	}
	case ClauseType::ClauseType::WITH: {
		ClauseWithObject* derivedWith = dynamic_cast<ClauseWithObject*>(current);
		return derivedWith->getRefObject1()->getSynonym();
	}
	case ClauseType::ClauseType::SUCH_THAT: {
		ClauseSuchThatObject* derivedSuchThat = dynamic_cast<ClauseSuchThatObject*>(current);
		if (derivedSuchThat->getArgsOne()->getIsSynonym()) {
			return derivedSuchThat->getArgsOne()->getStringValue();
		}
		else {
			return "";
		}
		//break;
	}

	}

	return "";
}

std::string QueryOptimization::getSecondSynonym(ClauseObject* current) {
	switch (current->getClauseType()) {

	case ClauseType::ClauseType::PATTERN: {
		// Pattern second arg is always an expression
		ClausePatternObject* derivedPattern = dynamic_cast<ClausePatternObject*>(current);
		if (derivedPattern->getIsFirstArgSynonym()) {
			return derivedPattern->getFirstArgument();
		}
		return "";

	}
	case ClauseType::ClauseType::WITH: {
		ClauseWithObject* derivedWith = dynamic_cast<ClauseWithObject*>(current);
		return derivedWith->getRefObject2()->getSynonym();
	}
	case ClauseType::ClauseType::SUCH_THAT: {
		ClauseSuchThatObject* derivedSuchThat = dynamic_cast<ClauseSuchThatObject*>(current);
		if (derivedSuchThat->getArgsTwo()->getIsSynonym()) {
			return derivedSuchThat->getArgsTwo()->getStringValue();
		}
		else {
			return "";
		}
	}
	}
	return "";

}

bool QueryOptimization::hasSynonymUsedBefore(ClauseObject* current, std::string synonym) {

	switch (current->getClauseType()) {

	case ClauseType::ClauseType::PATTERN: {
		ClausePatternObject* derivedPattern = dynamic_cast<ClausePatternObject*>(current);
		if (derivedPattern->getIsFirstArgSynonym()) {
			return derivedPattern->getFirstArgument().compare(synonym) == 0;
		}
		else {
			return false;
		}

	}
	case ClauseType::ClauseType::WITH: {
		ClauseWithObject* derivedWith = dynamic_cast<ClauseWithObject*>(current);

		return derivedWith->getRefObject1()->getSynonym().compare(synonym) == 0 ||
			derivedWith->getRefObject2()->getSynonym().compare(synonym) == 0;

	}
	case ClauseType::ClauseType::SUCH_THAT: {
		ClauseSuchThatObject* derivedSuchThat = dynamic_cast<ClauseSuchThatObject*>(current);

		if (derivedSuchThat->getArgsOne()->getIsSynonym()) {
			return derivedSuchThat->getArgsOne()->getStringValue().compare(synonym) == 0;
		}

		if (derivedSuchThat->getArgsTwo()->getIsSynonym()) {
			return derivedSuchThat->getArgsTwo()->getStringValue().compare(synonym) == 0;
		}

		return false;

	}
	}

	return false;

}






