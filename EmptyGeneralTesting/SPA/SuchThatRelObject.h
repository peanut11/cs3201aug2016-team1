#pragma once
#include "RelationshipType.h"
#include "SuchThatArgObject.h"

class SuchThatRelObject {

	RelationshipType type;
	SuchThatArgObject argsOne;
	SuchThatArgObject argsTwo;

public:
	SuchThatRelObject createSuchThatRelobject(RelationshipType type, SuchThatArgObject argsOne, SuchThatArgObject argsTwo);
	RelationshipType getRelationshipType();
	SuchThatArgObject getArgsOne();
	SuchThatArgObject getArgsTwo();
};