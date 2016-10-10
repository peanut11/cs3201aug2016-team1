#pragma once
#include "ClauseType.h"

class ClauseObject {

protected:
	ClauseObject();
	ClauseType::ClauseType mClauseType;

public:
	//virtual ~ClauseObject();
	ClauseType::ClauseType getClauseType();
	virtual ClauseObject* const clone()  = 0;

};