#pragma once
#include <vector>
#include <set>
#include "SelectObject.h"
#include "ClauseSuchThatObject.h"
#include "ClauseWithObject.h"
#include "ClausePatternObject.h"

class QueryTable {

	SelectObject select;
//	std::vector<ClauseObject> clauses;
	std::set<ClauseSuchThatObject> suchThats;
	std::set<ClauseWithObject> withs;
	std::set<ClausePatternObject> patterns;

public:
	SelectObject getSelect();
	std::set<ClauseSuchThatObject> getSuchThats();
	std::set<ClauseWithObject> getWiths();
	std::set<ClausePatternObject> getPatterns();
//	std::vector<ClauseObject> getClauses();

	bool replaceSelectObject(SelectObject object);
	bool insertSuchThatRelObject(ClauseSuchThatObject object);
	bool insertWithObject(ClauseWithObject object);
	bool insertPatternObject(ClausePatternObject object);



};
