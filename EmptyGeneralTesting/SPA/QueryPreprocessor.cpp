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

	this->validator = QueryValidator::getInstance();
	this->mRelTable = RelTable::getInstance();
	this->mSynonymTable = SynonymTable::getInstance();
	this->populateRelationshipTable();
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

	object = this->createRelationshipObject(RelationshipType::PARENT_STAR,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::FOLLOWS,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::FOLLOWS_STAR,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	// havent include next and affects
	object = this->createRelationshipObject(RelationshipType::NEXT,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN,EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN,EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::NEXT_STAR,
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN,EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::STMT, EntityType::IF, EntityType::WHILE, EntityType::CALL, EntityType::ASSIGN,EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::AFFECTS,
	{ EntityType::ASSIGN, EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::ASSIGN, EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

	object = this->createRelationshipObject(RelationshipType::AFFECTS_STAR,
	{ EntityType::ASSIGN, EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
	{ EntityType::ASSIGN, EntityType::PROGRAM_LINE, EntityType::CONSTANT, EntityType::WILDCARD },
		2);
	this->mRelTable->insert(object);

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

QueryTable QueryPreProcessor::getQueryTable() {
	return this->validator->getQueryTable();
}

bool QueryPreProcessor::isValidQuery(std::string queryString) {
	return this->validator->isValidQuery(queryString);
}