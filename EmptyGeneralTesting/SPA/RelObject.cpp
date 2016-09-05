#include "RelObject.h"


RelObject::RelObject(RelationshipType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs) {
	this->type = type;
	this->argsOneTypes = argsOneTypes;
	this->argsTwoTypes = argsTwoTypes;
	this->numOfArgs = numOfArgs;
}

RelationshipType RelObject::getRelObjectType() {
	return this->type;
}

std::vector<EntityType> RelObject::getArgsTwoTypes() {
	return this->argsTwoTypes;
}

bool RelObject::doesFirstArgsContains(EntityType type) {
	if (type == EntityType::INVALID) { return false;  }
	for (auto value : this->argsOneTypes) {
		if (value == type) {
			return true;
		}
	}
	return false;
}

bool RelObject::doesSecondArgsContains(EntityType type) {
	if (type == EntityType::INVALID) { return false; }
	for (auto value : this->argsTwoTypes) {
		if (value == type) {
			return true;
		}
	}
	return false;
}

int RelObject::getNumOfArgs() {
	return this->numOfArgs;
}