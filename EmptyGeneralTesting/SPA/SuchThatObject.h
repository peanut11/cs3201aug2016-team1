#pragma once
#include <vector>
#include "ClauseObject.h"
#include "SuchThatRelObject.h"

class SuchThatObject: public ClauseObject {
	std::vector<SuchThatRelObject> relationships;

public:
	std::vector<SuchThatRelObject> getRelationships();
};