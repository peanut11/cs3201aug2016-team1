#pragma once
#include <string>
#include "QueryValidator.h"
#include "SynonymTable.h"
#include "SynonymObject.h"
#include "RelTable.h"
#include "RelObject.h"
#include "RelationshipType.h"
#include "EntityType.h"

class QueryPreProcessor {

	static QueryPreProcessor *_instance;

	QueryValidator *validator;

	SynonymTable *mSynonymTable;
	RelTable *mRelTable;

	void init();
	void populateRelationshipTable();
	//void populateSynonymTable();
	SynonymObject createSynonymObject(EntityType type, std::string synonym);
	RelObject createRelationshipObject(RelationshipType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs);
	


public:
	static QueryPreProcessor *getInstance();
	SynonymTable *getSynonymTable();
	RelTable *getRelationshipTable();
	QueryTable getQueryTable();
	EntityType *getRelationType();

	bool isValidQuery(std::string queryString);

};



