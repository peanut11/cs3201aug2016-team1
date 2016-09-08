#pragma once
#include <vector>
#include <set>
#include "SelectObject.h"
#include "SuchThatRelObject.h"
#include "WithObject.h"
#include "PatternObject.h"
//#include "ClauseObject.h"

class QueryTable {

	SelectObject select;
//	std::vector<ClauseObject> clauses;
	std::set<SuchThatRelObject> suchThats;
	std::set<WithObject> withs;
	std::set<PatternObject> patterns;

public:
	SelectObject getSelect();
	std::set<SuchThatRelObject> getSuchThats();
	std::set<WithObject> getWiths();
	std::set<PatternObject> getPatterns();
//	std::vector<ClauseObject> getClauses();

};
