#pragma once
#include "ClauseWithRefObject.h"

class ClauseWithObject {
	ClauseWithRefObject obj1;
	ClauseWithRefObject obj2;
	bool isResultsBoolean = false;
public:

	ClauseWithObject();
	ClauseWithObject(ClauseWithRefObject obj1, ClauseWithRefObject obj2);

	ClauseWithRefObject getRefObject1();
	ClauseWithRefObject getRefObject2();
	bool getResultsBoolean();
	void setResultsBoolean(bool boolean);

};