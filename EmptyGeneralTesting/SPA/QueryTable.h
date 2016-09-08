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
	std::vector<ClauseSuchThatObject> suchThats;
	std::vector<ClauseWithObject> withs;
	std::vector<ClausePatternObject> patterns;

public:
	SelectObject getSelect();
	std::vector<ClauseSuchThatObject> getSuchThats();
	std::vector<ClauseWithObject> getWiths();
	std::vector<ClausePatternObject> getPatterns();
//	std::vector<ClauseObject> getClauses();

	bool replaceSelectObject(SelectObject object);
	bool insertSuchThatRelObject(ClauseSuchThatObject object);
	bool insertWithObject(ClauseWithObject object);
	bool insertPatternObject(ClausePatternObject object);



};
