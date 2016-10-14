#pragma once

#include "RelationshipPopulator.h"

std::set<StmtNumber> RelationshipPopulator::getNextStar(StmtNumber startStmt, StmtNumber endStmt)
{
	std::set<StmtNumber> results;
	if (startStmt == 0 && endStmt == 0) return results;
	
	PKB* pkb = PKB::getInstance();
	std::vector<StmtNumber> nextStmts;
	std::set<StmtNumber> potentialNextStmts;
	std::set<StmtNumber> whileChildren;
	std::set<StmtNumber> grandparents;
	std::set<StmtNumber>::const_iterator grandparentsIt;
	bool isTopDown = (startStmt!= 0);
	StmtNumber currentStmt;
	StmtNumber oldestWhile = 0;

	if (isTopDown) { 
		potentialNextStmts = pkb->getStmtsByStmt(NEXT, startStmt);
	} else { 
		potentialNextStmts = pkb->getStmtsByStmt(endStmt, NEXT);
	}
	nextStmts.insert(nextStmts.begin(), potentialNextStmts.begin(), potentialNextStmts.end());
	
	while (!nextStmts.empty()) {
		currentStmt = nextStmts.back();
		nextStmts.pop_back();
		results.insert(currentStmt);

		// Search for oldest while
		if (pkb->getStmtTypeForStmt(currentStmt) == WHILE) oldestWhile = currentStmt;
		grandparents = pkb->getStmtsByStmt(currentStmt, PARENT_STAR);
		grandparentsIt = grandparents.begin();
		if (!grandparents.empty()) {
			while (grandparentsIt != grandparents.end()) {
				if (pkb->getStmtTypeForStmt(*grandparentsIt) == WHILE) {
					oldestWhile = *grandparentsIt;
				}
				grandparentsIt++;
			}
		}

		if (oldestWhile != 0) {
			results.insert(oldestWhile);
			// Adds all children of oldest while into results
			whileChildren = pkb->getStmtsByStmt(PARENT_STAR, oldestWhile);
			if (isTopDown && whileChildren.find(endStmt) != whileChildren.end()) {
				// Children of while contains endStmt
				results.insert(whileChildren.begin(), whileChildren.find(endStmt));
				break;
			} else {
				results.insert(whileChildren.begin(), whileChildren.end());
			}
			
			// Adds follower of while into next
			if (isTopDown) {
				potentialNextStmts = pkb->getStmtsByStmt(FOLLOWS, oldestWhile);
			} else {
				potentialNextStmts = pkb->getStmtsByStmt(oldestWhile, FOLLOWS);
				if (potentialNextStmts.empty()) {
					potentialNextStmts = pkb->getStmtsByStmt(oldestWhile, PARENT);
				}
			}

			
			oldestWhile = 0;
		} else {
			if (currentStmt == endStmt) break;
			if (isTopDown) {
				potentialNextStmts = pkb->getStmtsByStmt(NEXT, currentStmt); // Gets next
			} else {
				potentialNextStmts = pkb->getStmtsByStmt(currentStmt, NEXT); // Gets previous
			}
		}
		
		nextStmts.insert(nextStmts.end(), potentialNextStmts.begin(), potentialNextStmts.end());
		
	}
	return results;
}
