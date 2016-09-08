#pragma once
#include <vector>
#include <set>
#include "SelectObject.h"
#include "ClauseSuchThatObject.h"
#include "WithObject.h"
#include "ClausePatternObject.h"
//#include "ClauseObject.h"

class QueryTable {

	SelectObject select;
//	std::vector<ClauseObject> clauses;
	std::set<ClauseSuchThatObject> suchThats;
	std::set<WithObject> withs;
	std::set<ClausePatternObject> patterns;

public:
	SelectObject getSelect();
	std::set<ClauseSuchThatObject> getSuchThats();
	std::set<WithObject> getWiths();
	std::set<ClausePatternObject> getPatterns();
//	std::vector<ClauseObject> getClauses();

	bool replaceSelectObject(SelectObject object);
	bool insertSuchThatRelObject(ClauseSuchThatObject object);
	bool insertWithObject(WithObject object);
	bool insertPatternObject(ClausePatternObject object);



};
