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
	ClauseSuchThatObject();
	ClauseSuchThatObject (RelationshipType type, ClauseSuchThatArgObject argsOne, ClauseSuchThatArgObject argsTwo);
	
	RelationshipType getRelationshipType();
	ClauseSuchThatArgObject getArgsOne();
	ClauseSuchThatArgObject getArgsTwo();
	bool getResultsBoolean();
	void setResultsBoolean(bool boolean);
	
};
