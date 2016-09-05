#pragma once
#include <vector>
#include "SelectObject.h"
#include "ClauseObject.h"

class QueryTree {

	SelectObject select;
	std::vector<ClauseObject> clauses;

public:
	SelectObject getSelect();
	std::vector<ClauseObject> getClauses();

};
