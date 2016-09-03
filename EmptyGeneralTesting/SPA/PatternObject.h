#pragma once
#include <vector>
#include "ClauseObject.h"
#include "PatternTypeObject.h"

class PatternObject : public ClauseObject {
	std::vector<PatternTypeObject> patterns;

public:
	std::vector<PatternTypeObject> getPatterns();
};