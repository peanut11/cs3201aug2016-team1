#pragma once
#include "RelationshipType.h"
#include "SuchThatArgObject.h"

class SuchThatRelObject {

	RelationshipType type;
	SuchThatArgObject argsOne;
	SuchThatArgObject argsTwo;
	bool isResultsBoolean = false;

public:
	SuchThatRelObject createSuchThatRelobject(RelationshipType type, SuchThatArgObject argsOne, SuchThatArgObject argsTwo);
	RelationshipType getRelationshipType();
	SuchThatArgObject getArgsOne();
	SuchThatArgObject getArgsTwo();
	void setResultsBoolean(bool boolean);
	bool getResultsBoolean();
};