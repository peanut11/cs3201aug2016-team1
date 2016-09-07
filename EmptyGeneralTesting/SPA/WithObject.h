#pragma once
#include <vector>
#include "ClauseObject.h"
#include "WithAttrObject.h"

class WithObject: public ClauseObject {
	std::vector<WithAttrObject> attributes;

public:
	std::vector<WithAttrObject> getAttributes();
	void evaluate();
};