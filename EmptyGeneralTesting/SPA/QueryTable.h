#pragma once
#include <vector>
#include <set>
#include "ClauseSelectResultObject.h"
#include "ClauseSelectObject.h"
#include "ClauseSuchThatObject.h"
#include "ClauseWithObject.h"
#include "ClausePatternObject.h"

class QueryTable {

	
	ClauseSelectResultObject result;
	//ClauseSelectObject select;
	std::vector<ClauseSuchThatObject*> suchThats;
	std::vector<ClauseWithObject*> withs;
	std::vector<ClausePatternObject*> patterns;
	
	std::string getEntityString(EntityType type);
	std::string getRelationshipString(RelationshipType type);
	std::string getAttrString(AttrType::AttrType type);
	std::string getWithRefString(WithRefType type);

public:
	QueryTable();
	//~QueryTable();

	void clearAll();
	
	ClauseSelectResultObject getResult();
	//ClauseSelectObject getSelect();
	std::vector<ClauseSuchThatObject*>& getSuchThats();
	std::vector<ClauseWithObject*> getWiths();
	std::vector<ClausePatternObject*> getPatterns();

	std::string toString();
	
	bool insertSelectObject(ClauseSelectObject object);
	bool insertSuchThatObject(ClauseSuchThatObject* object);
	bool insertWithObject(ClauseWithObject* object);
	bool insertPatternObject(ClausePatternObject* object);



};
