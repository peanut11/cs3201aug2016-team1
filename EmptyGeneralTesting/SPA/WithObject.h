#pragma once
#include <vector>
#include "WithAttrObject.h"

class WithObject{
	std::vector<WithAttrObject> attributes;

public:
	std::vector<WithAttrObject> getAttributes();
	void evaluate();
};
