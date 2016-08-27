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
	this->populateRelationshipTable();
}

void QueryPreProcessor::populateRelationshipTable() {

	RelObject object = this->createRelationshipObjects(RelObjectType::CALLS,
	{ RelArgsType::SYNONYM_PROC, RelArgsType::STRING, RelArgsType::UNDERSCORE },
	{ RelArgsType::SYNONYM_PROC, RelArgsType::STRING, RelArgsType::UNDERSCORE },
		2);

	this->mRelTable->insert(object);
}

RelObject QueryPreProcessor::createRelationshipObjects(RelObjectType type, std::vector<RelArgsType> argsOneTypes, std::vector<RelArgsType> argsTwoTypes, int numOfArgs) {
	RelObject object = RelObject(type, argsOneTypes, argsTwoTypes, numOfArgs);
	return object;
}

RelTable *QueryPreProcessor::getRelationshipTable() {
	return this->mRelTable;
}