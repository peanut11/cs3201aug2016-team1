#pragma once
#include "ClauseObject.h"
#include "RelationshipType.h"
#include "ClauseSuchThatArgObject.h"

class ClauseSuchThatObject: public ClauseObject {
private:

	RelationshipType type;
	ClauseSuchThatArgObject* argsOne;
	ClauseSuchThatArgObject* argsTwo;
	bool isResultsBoolean = false;

	int numOfSynonym = 0;

public:

	ClauseSuchThatObject();
	
	ClauseSuchThatObject (RelationshipType type, ClauseSuchThatArgObject* argsOne, ClauseSuchThatArgObject* argsTwo);
	ClauseSuchThatObject* clone() { return new ClauseSuchThatObject(*this); }

	RelationshipType getRelationshipType() const;
	ClauseSuchThatArgObject* getArgsOne() const;
	ClauseSuchThatArgObject* getArgsTwo() const;
	bool getResultsBoolean();
	int getNumberOfSynonym();
	void setResultsBoolean(bool boolean);
	
};
