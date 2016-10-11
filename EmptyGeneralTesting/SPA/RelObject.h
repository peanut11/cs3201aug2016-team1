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

	RelObject(RelationshipType type, std::vector<EntityType> argsOneTypes, std::vector<EntityType> argsTwoTypes, int numOfArgs);

	void setRelationshipType(RelationshipType type);

	RelationshipType getRelObjectType();
	std::vector<EntityType> getArgsOneTypes();
	std::vector<EntityType> getArgsTwoTypes();
	bool doesFirstArgsContains(EntityType type);
	bool doesSecondArgsContains(EntityType type);
	int getNumOfArgs();

};
