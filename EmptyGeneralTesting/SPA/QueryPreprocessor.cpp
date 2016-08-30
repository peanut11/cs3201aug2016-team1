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
	{ RelArgsType::SYNONYM_PROC, RelArgsType::STRING, RelArgsType::UNDERSCORE },
	{ RelArgsType::SYNONYM_PROC, RelArgsType::STRING, RelArgsType::UNDERSCORE },
		2);

	this->mRelTable->insert(object);
}

void QueryPreProcessor::populateSynonymTable() {
	SynonymObject object = this->createSynonymObject(RelArgsType::SYNONYM_PROC, "p");
	this->mSynonymTable->insert(object);
	object = this->createSynonymObject(RelArgsType::SYNONYM_ASSIGN, "a1");
	this->mSynonymTable->insert(object);
}

SynonymObject QueryPreProcessor::createSynonymObject(RelArgsType type, std::string synonym) {
	SynonymObject object = SynonymObject(type, synonym);
	return object;
}

RelObject QueryPreProcessor::createRelationshipObject(RelObjectType type, std::vector<RelArgsType> argsOneTypes, std::vector<RelArgsType> argsTwoTypes, int numOfArgs) {
	RelObject object = RelObject(type, argsOneTypes, argsTwoTypes, numOfArgs);
	return object;
}

SynonymTable *QueryPreProcessor::getSynonymTable() {
	return this->mSynonymTable;
}

RelTable *QueryPreProcessor::getRelationshipTable() {
	return this->mRelTable;
}