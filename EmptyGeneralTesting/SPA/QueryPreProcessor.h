#pragma once
#include <string>
#include "SynonymTable.h"
#include "SynonymObject.h"
#include "RelTable.h"
#include "RelObject.h"
#include "RelObjectType.h"
#include "RelArgsType.h"

class QueryPreProcessor {

	static QueryPreProcessor *_instance;
	SynonymTable *mSynonymTable;
	RelTable *mRelTable;

	void init();
	void populateRelationshipTable();
	void populateSynonymTable();
	SynonymObject createSynonymObject(RelArgsType type, std::string synonym);
	RelObject createRelationshipObject(RelObjectType type, std::vector<RelArgsType> argsOneTypes, std::vector<RelArgsType> argsTwoTypes, int numOfArgs);
	


public:
	static QueryPreProcessor *getInstance();
	SynonymTable *getSynonymTable();
	RelTable *getRelationshipTable();

};



