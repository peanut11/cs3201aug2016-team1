#pragma once
#include "RelationshipType.h"
#include "ClauseSuchThatArgObject.h"

class ClauseSuchThatObject {
private:
	RelationshipType type;
	ClauseSuchThatArgObject argsOne;
	ClauseSuchThatArgObject argsTwo;
	bool isResultsBoolean = false;

public:
	ClauseSuchThatObject createSuchThatRelobject(RelationshipType type, ClauseSuchThatArgObject argsOne, ClauseSuchThatArgObject argsTwo);
	RelationshipType getRelationshipType();
	ClauseSuchThatArgObject getArgsOne();
	ClauseSuchThatArgObject getArgsTwo();
	void setResultsBoolean(bool boolean);
	bool getResultsBoolean();
};
