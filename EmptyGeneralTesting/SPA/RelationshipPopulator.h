#pragma once
#include "PKB.h"

class RelationshipPopulator {

public:
	static std::set<StmtNumber> getNextStar(StmtNumber startStmt, StmtNumber endStmt);
};