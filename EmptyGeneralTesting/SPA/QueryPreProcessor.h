#pragma once
#include "RelTable.h"
#include "RelObject.h"
#include "RelObjectType.h"
#include "RelArgsType.h"

class QueryPreProcessor {

	static QueryPreProcessor *_instance;
	RelTable *mRelTable;

	void init();
	void populateRelationshipTable();
	RelObject createRelationshipObjects(RelObjectType type, std::vector<RelArgsType> argsOneTypes, std::vector<RelArgsType> argsTwoTypes, int numOfArgs);
	


public:
	static QueryPreProcessor *getInstance();
	RelTable *getRelationshipTable();

};



