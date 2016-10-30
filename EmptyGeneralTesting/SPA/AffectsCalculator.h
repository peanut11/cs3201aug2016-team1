#pragma once

#include "Types.h"
#include <vector>
#include <set>

class AffectsCalculator {
private:
	static AffectsCalculator* instance;
	int stmtCount;
	std::vector<std::set<VarIndex> > visited[2];

	StmtNumber dest;
	StmtSet results;
	std::vector<StmtNumber> affectsStack;
	std::vector< std::vector<int> > cache;
	bool dfs		(bool isBegin, StmtNumber x, VarIndex v, bool isStar);
	void dfsAffected(bool isBegin, StmtNumber x, VarIndex v, bool isStar);
	void dfsAffectors(bool isEnd, StmtNumber x, VarIndex v, bool isStar);

public:
	AffectsCalculator();
	static AffectsCalculator* getInstance();
	void clear();
	bool isAffects(StmtNumber x, StmtNumber y, bool isStar);
	StmtSet getAffected(StmtNumber x, bool isStar);
	StmtSet getAffectors(StmtNumber x, bool isStar);
};