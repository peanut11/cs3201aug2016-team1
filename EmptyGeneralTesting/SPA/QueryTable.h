#pragma once
#include <vector>
#include "SelectObject.h"
#include "SuchThatRelObject.h"
#include "WithObject.h"
#include "PatternObject.h"
//#include "ClauseObject.h"

class QueryTable {

	SelectObject select;
//	std::vector<ClauseObject> clauses;
	std::vector<SuchThatRelObject> suchThats;
	std::vector<WithObject> withs;
	std::vector<PatternObject> patterns;

public:
	SelectObject getSelect();
	std::vector<SuchThatRelObject> getSuchThats();
	std::vector<WithObject> getWiths();
	std::vector<PatternObject> getPatterns();
//	std::vector<ClauseObject> getClauses();

};
