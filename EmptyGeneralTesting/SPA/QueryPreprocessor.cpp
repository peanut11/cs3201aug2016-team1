// Maintained by: Gerald
//
// Accessed by:
// - QueryProcessor
//
// Constructs:
// - QueryTree
//   - QueryNode

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

	RelObject object = this->createRelationshipObject(RelationshipType::MODIFIES,
	{ EntityType::PROCEDURE, EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT },	// args 1
	{ EntityType::VARIABLE, EntityType::WILDCARD },						// args 2
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::USES,
	{ EntityType::PROCEDURE, EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT },	// args 1
	{ EntityType::VARIABLE, EntityType::WILDCARD },						// args 2
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::CALLS,
	{ EntityType::PROCEDURE, EntityType::WILDCARD },
	{ EntityType::PROCEDURE, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::CALLSSTAR,
	{ EntityType::PROCEDURE, EntityType::WILDCARD },
	{ EntityType::PROCEDURE, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::PARENT,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::PARENTSTAR,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::FOLLOWS,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::FOLLOWSSTAR,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	// havent include next and affects
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

RelObject QueryPreProcessor::createRelationshipObject(RelationshipType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs) {
	RelObject object = RelObject(type, argsOneTypes, argsTwoTypes, numOfArgs);
	return object;
}

SynonymTable *QueryPreProcessor::getSynonymTable() {
	return this->mSynonymTable;
}

RelTable *QueryPreProcessor::getRelationshipTable() {
	return this->mRelTable;
}