#pragma once
#include "ClauseObject.h"
#include "ClauseWithRefObject.h"

class ClauseWithObject: public ClauseObject {
	ClauseWithRefObject* obj1;
	ClauseWithRefObject* obj2;
	bool isResultsBoolean = false;
public:

	ClauseWithObject();
	ClauseWithObject(ClauseWithRefObject* obj1, ClauseWithRefObject* obj2);

	ClauseWithObject* clone() { return new ClauseWithObject(*this); }

	ClauseWithRefObject* getRefObject1();
	ClauseWithRefObject* getRefObject2();
	bool getResultsBoolean();
	void setResultsBoolean(bool boolean);

};