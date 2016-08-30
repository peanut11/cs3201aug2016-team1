// Maintained by: Gerald
//
// Accessed by:
// - QueryProcessor
//
// Constructs:
// - QueryTree
//   - QueryTNode

#include "QueryPreProcessor.h"

QueryPreProcessor *QueryPreProcessor::_instance;

/*
Singleton
*/
QueryPreProcessor *QueryPreProcessor::getInstance()
{
	if (!_instance)
		_instance = new QueryPreProcessor;
	_instance->init();
	return _instance;
}


void QueryPreProcessor::init() {
	this->mRelTable = RelTable::getInstance();
	this->mSynonymTable = SynonymTable::getInstance();
	this->populateRelationshipTable();
	this->populateSynonymTable();
}

void QueryPreProcessor::populateRelationshipTable() {

	RelObject object = this->createRelationshipObject(RelObjectType::CALLS,
	{ EntityType::PROCEDURE, EntityType::VARIABLE, EntityType::WILDCARD },
	{ EntityType::PROCEDURE, EntityType::VARIABLE, EntityType::WILDCARD },
		2);

	this->mRelTable->insert(object);
}

void QueryPreProcessor::populateSynonymTable() {
	SynonymObject object = this->createSynonymObject(EntityType::PROCEDURE, "p");
	this->mSynonymTable->insert(object);
	object = this->createSynonymObject(EntityType::ASSIGN, "a1");
	this->mSynonymTable->insert(object);
}

SynonymObject QueryPreProcessor::createSynonymObject(EntityType type, std::string synonym) {
	SynonymObject object = SynonymObject(type, synonym);
	return object;
}

RelObject QueryPreProcessor::createRelationshipObject(RelObjectType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs) {
	RelObject object = RelObject(type, argsOneTypes, argsTwoTypes, numOfArgs);
	return object;
}

SynonymTable *QueryPreProcessor::getSynonymTable() {
	return this->mSynonymTable;
}

RelTable *QueryPreProcessor::getRelationshipTable() {
	return this->mRelTable;
}