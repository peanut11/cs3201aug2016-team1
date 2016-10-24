#pragma once
#include <vector>
#include <set>
#include "Types.h"

class RelationshipPopulator {
private:
	static RelationshipPopulator* theOne;
	std::vector<std::set<StmtNumber>> nextStars;
	std::vector<bool> doneNextFlags; // should be initialized with fals
	std::vector<std::set<StmtNumber>> previousStars;
	std::vector<bool> donePreviousFlags; // should be initialized with false

	void storeNextStar(StmtNumber stmt, std::set<StmtNumber> nextStar);
	void storePrevStar(StmtNumber stmt, std::set<StmtNumber> prevStar);
	StmtNumber getOldestWhile(StmtNumber stmt);
public:
	static RelationshipPopulator* getInstance();
	void clear();
	RelationshipPopulator();
	std::set<StmtNumber> getAndMemoiseNextStar(bool isNext, StmtNumber stmt);
	/*std::set<StmtNumber> getNextStar(StmtNumber startStmt, StmtNumber endStmt);*/
	bool isNextStar(StmtNumber startStmt, StmtNumber endStmt);
};