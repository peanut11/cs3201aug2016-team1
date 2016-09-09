#pragma once
#include <vector>
#include <set>
#include "SelectObject.h"
#include "ClauseSuchThatObject.h"
#include "ClauseWithObject.h"
#include "ClausePatternObject.h"

class QueryTable {

	SelectObject select;
	std::vector<ClauseSuchThatObject> suchThats;
	std::vector<ClauseWithObject> withs;
	std::vector<ClausePatternObject> patterns;

public:
	QueryTable();
	SelectObject getSelect();
	std::vector<ClauseSuchThatObject> getSuchThats();
	std::vector<ClauseWithObject> getWiths();
	std::vector<ClausePatternObject> getPatterns();

	bool replaceSelectObject(SelectObject object);
	bool insertSuchThatObject(ClauseSuchThatObject object);
	bool insertWithObject(ClauseWithObject object);
	bool insertPatternObject(ClausePatternObject object);



};
