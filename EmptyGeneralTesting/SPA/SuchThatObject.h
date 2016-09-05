#pragma once
#include <vector>
#include "ClauseObject.h"
#include "RelObject.h"

class SuchThatObject: public ClauseObject {
	std::vector<RelObject> relationships;

public:
	std::vector<RelObject> getRelationships();
};