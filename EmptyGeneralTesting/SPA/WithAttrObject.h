#pragma once
#include "WithRefObject.h"

class WithAttrObject {
	WithRefObject obj1;
	WithRefObject obj2;

public:
	WithRefObject getRefObject1();
	WithRefObject getRefObject2();
};