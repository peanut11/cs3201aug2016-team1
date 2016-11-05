#pragma once
#include <vector>
#include <set>
#include "Types.h"

class RelationshipPopulator {
private:
	static RelationshipPopulator* theOne;
	std::vector<std::set<StmtNumber>> nextStars;
	std::vector<bool> hasNextStars; // should be initialized with fals
	std::vector<std::set<StmtNumber>> previousStars;
	std::vector<bool> hasPrevStars; // should be initialized with false

	void storeNextStar(StmtNumber stmt, std::set<StmtNumber> nextStar);
	void storePrevStar(StmtNumber stmt, std::set<StmtNumber> prevStar);
	StmtNumber getOldestWhile(StmtNumber stmt);
	StmtNumber getFollowsOfWhile(StmtNumber stmt);
	StmtNumber getLeaderOfWhile(StmtNumber stmt);
	StmtNumber getLastOfMemoiseNextStar(StmtNumber stmt);
	StmtNumber getFirstOfMemoisePrevStar(StmtNumber stmt);
public:
	static RelationshipPopulator* getInstance();
	void clear();
	RelationshipPopulator();
	std::set<StmtNumber> getNextStar(StmtNumber startStmt, StmtNumber endStmt);
};