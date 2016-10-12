#pragma once

#include "RelationshipPopulator.h"

std::set<StmtNumber> RelationshipPopulator::getNextStar(StmtNumber startStmt, StmtNumber endStmt)
{
	std::set<StmtNumber> results;
	if (startStmt == 0 && endStmt == 0) return results;
	
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> nextStmts;
	std::set<StmtNumber> potentialNextStmts;
	bool isTopDown = (startStmt!= 0);

	if (isTopDown) { 
		nextStmts = pkb->getStmtsByStmt(NEXT, startStmt);
	} else { 
		nextStmts = pkb->getStmtsByStmt(endStmt, NEXT);
	}

	std::set<StmtNumber>::const_iterator nextIt = nextStmts.begin();
	while (nextIt != nextStmts.end()) {
		results.insert(*nextIt);
		if (*nextIt == endStmt) break;
		if (isTopDown) {
			potentialNextStmts = pkb->getStmtsByStmt(NEXT, *nextIt);
		}
		else {
			potentialNextStmts = pkb->getStmtsByStmt(*nextIt, NEXT);
		}
		
		nextStmts.insert(potentialNextStmts.begin(), potentialNextStmts.end());
		nextIt++;
	}
	return results;
}
