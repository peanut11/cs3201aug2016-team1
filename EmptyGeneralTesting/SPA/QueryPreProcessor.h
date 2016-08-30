#pragma once
#include <string>
#include "SynonymTable.h"
#include "SynonymObject.h"
#include "RelTable.h"
#include "RelObject.h"
#include "RelObjectType.h"
#include "EntityType.h"

class QueryPreProcessor {

	static QueryPreProcessor *_instance;
	SynonymTable *mSynonymTable;
	RelTable *mRelTable;

	void init();
	void populateRelationshipTable();
	void populateSynonymTable();
	SynonymObject createSynonymObject(EntityType type, std::string synonym);
	RelObject createRelationshipObject(RelObjectType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs);
	


public:
	static QueryPreProcessor *getInstance();
	SynonymTable *getSynonymTable();
	RelTable *getRelationshipTable();
	EntityType *getRelationType();

};



