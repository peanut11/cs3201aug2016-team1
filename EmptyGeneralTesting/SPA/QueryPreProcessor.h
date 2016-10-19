#pragma once

#include <string>

#include "EntityType.h"
#include "QueryValidator.h"
#include "RelationshipType.h"
#include "RelObject.h"
#include "RelTable.h"
#include "SynonymObject.h"
#include "SynonymTable.h"

class QueryPreProcessor {
	static QueryPreProcessor *_instance;
    QueryPreProcessor();

	QueryValidator *validator;
    RelTable *mRelTable;
    SynonymTable *mSynonymTable;

	RelObject createRelationshipObject(RelationshipType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs);
    SynonymObject createSynonymObject(EntityType type, std::string synonym);
    void populateRelationshipTable();
	
public:
	static QueryPreProcessor *getInstance();
	
    EntityType*   getRelationType();
    QueryTable    getQueryTable();
    RelTable*     getRelationshipTable();
    SynonymTable* getSynonymTable();

	bool isValidQuery(std::string queryString);
};
