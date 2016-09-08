#pragma once
#include "ClauseWithRefObject.h"

class ClauseWithObject {
	ClauseWithRefObject obj1;
	ClauseWithRefObject obj2;

public:

	ClauseWithObject();
	ClauseWithObject(ClauseWithRefObject obj1, ClauseWithRefObject obj2);

	ClauseWithRefObject getRefObject1();
	ClauseWithRefObject getRefObject2();
};