#pragma once
#include<stdio.h>
#include <vector>
#include "RelationshipType.h"
#include "EntityType.h"


class RelObject {
	
	RelationshipType type;
	std::vector<EntityType> argsOneTypes;
	std::vector<EntityType> argsTwoTypes;
	int numOfArgs;

public:

	RelObject(RelationshipType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs) {
		this->type = type;
		this->argsOneTypes = argsOneTypes;
		this->argsTwoTypes = argsTwoTypes;
		this->numOfArgs = numOfArgs;
	}

	RelationshipType getRelObjectType() {
		return this->type;
	}

	std::vector<EntityType> getArgsOneTypes() {
		return this->argsOneTypes;
	}

	std::vector<EntityType> getArgsTwoTypes() {
		return this->argsTwoTypes;
	}

	int getNumOfArgs() {
		return this->numOfArgs;
	}

};
