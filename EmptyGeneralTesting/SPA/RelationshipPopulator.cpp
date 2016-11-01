#pragma once

#include "RelationshipPopulator.h"
#include "PKB.h"

RelationshipPopulator* RelationshipPopulator::theOne = nullptr;

RelationshipPopulator::RelationshipPopulator() {
	PKB* pkb = PKB::getInstance();
	size_t tableSize = pkb->getStmtTableSize();
	/*nextStars.reserve(tableSize+1);
	doneNextFlags.reserve(tableSize + 1);
	previousStars.reserve(tableSize + 1);
	donePreviousFlags.reserve(tableSize + 1);*/
	for (size_t i = 1; i <= tableSize + 1; i++) {
		nextStars.push_back(std::set<StmtNumber>());
		previousStars.push_back(std::set<StmtNumber>());
		doneNextFlags.push_back(false);
		donePreviousFlags.push_back(false);
	}
}

RelationshipPopulator* RelationshipPopulator::getInstance() {
	if (theOne == nullptr) {
		theOne = new RelationshipPopulator();
	}

	return theOne;
}

void RelationshipPopulator::clear() {
	if (theOne != nullptr) {
		delete theOne;
	}

	theOne = new RelationshipPopulator();
}

bool RelationshipPopulator::isNextStar(StmtNumber startStmt, StmtNumber endStmt)
{
	std::set<StmtNumber> results;
	if (startStmt == 0 && endStmt == 0) return false;
	
	PKB* pkb = PKB::getInstance();
	std::vector<StmtNumber> nextStmts;
	std::set<StmtNumber> potentialNextStmts;
	std::set<StmtNumber> whileChildren;
	std::set<StmtNumber> grandparents;
	std::set<StmtNumber>::const_iterator grandparentsIt;
	bool isTopDown = (startStmt!= 0);
	StmtNumber currentStmt;
	StmtNumber oldestWhile = 0;

	if (startStmt!= 0 && endStmt!= 0 && 
		pkb->getProcByStmt(startStmt) != pkb->getProcByStmt(endStmt)) return false;
 
	potentialNextStmts = pkb->getStmtsByStmt(NEXT, startStmt);
	
	nextStmts.insert(nextStmts.begin(), potentialNextStmts.begin(), potentialNextStmts.end());
	
	while (!nextStmts.empty()) {
		currentStmt = nextStmts.back();
		nextStmts.pop_back();
		results.insert(currentStmt);

		oldestWhile = getOldestWhile(currentStmt);

		if (oldestWhile != 0) {
			// Adds all children of oldest while into results
			whileChildren = pkb->getStmtsByStmt(PARENT_STAR, oldestWhile);
			whileChildren.insert(oldestWhile);
			if (whileChildren.find(endStmt) != whileChildren.end()) {
				// Children of while contains endStmt
				results.insert(whileChildren.begin(), whileChildren.find(endStmt));
				break;
			} else {
				results.insert(whileChildren.begin(), whileChildren.end());
			}
			
			// Adds follower of while into next
				potentialNextStmts = pkb->getStmtsByStmt(FOLLOWS, oldestWhile);
				if (potentialNextStmts.empty()) { 
					// While is the end of a stmtLst
					// Might be inside an if-else stmtLst
					potentialNextStmts = pkb->getStmtsByStmt(NEXT, oldestWhile);
				}
			oldestWhile = 0;
		} else {
			if (currentStmt == endStmt) break;
			potentialNextStmts = pkb->getStmtsByStmt(NEXT, currentStmt); // Gets next
			
		}
		
		nextStmts.insert(nextStmts.end(), potentialNextStmts.begin(), potentialNextStmts.end());
		
	}
	return !results.empty();
}

std::set<StmtNumber> RelationshipPopulator::getAndMemoiseNextStar(bool isNext, StmtNumber stmt) {
	std::set<StmtNumber> nextStmts;
	std::set<StmtNumber> potentialNextStmts;
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> results;
	StmtSetIterator it;
	StmtNumber oldestWhile = 0;
	std::set<StmtNumber> whileChildren;
	StmtSetIterator whileChildrenIt;
	if (isNext) {
		if (doneNextFlags[stmt]) {
			return nextStars[stmt];
		} else {
			oldestWhile = getOldestWhile(stmt);
			if (oldestWhile != 0) {
				whileChildren = pkb->getStmtsByStmt(PARENT_STAR, oldestWhile);
				whileChildren.insert(oldestWhile);
				results.insert(whileChildren.begin(), whileChildren.end());

				potentialNextStmts = pkb->getStmtsByStmt(FOLLOWS, oldestWhile);
                StmtNumber currentStmt = oldestWhile;
                bool isContinue = currentStmt != stmt;
				while (potentialNextStmts.empty() && isContinue) {
					// While is the end of a stmtLst
					// Might be at the end of many nested if-else stmtLst
                    StmtSet parentSet = pkb->getStmtsByStmt(currentStmt, PARENT);
                    if (!parentSet.empty()) {
                        StmtNumber parent = *parentSet.begin();
                        if (pkb->getStmtTypeForStmt(parent) == IF) {
                            StmtSet followSet = pkb->getStmtsByStmt(FOLLOWS, parent);
                            potentialNextStmts = followSet;
                        } else {
                            isContinue = false;
                        }
                    } else {
                        isContinue = false;
                    }
				}
				for (it = potentialNextStmts.begin(); it != potentialNextStmts.end(); it++) {
					results.insert(*it);
					nextStmts = getAndMemoiseNextStar(isNext, *it);
					results.insert(nextStmts.begin(), nextStmts.end());
				}
				for (whileChildrenIt = whileChildren.begin(); whileChildrenIt != whileChildren.end(); whileChildrenIt++) {
					storeNextStar(*whileChildrenIt, results);
				}
				return results;	
			} else {
				potentialNextStmts = pkb->getStmtsByStmt(NEXT, stmt); // Gets next
				for (it = potentialNextStmts.begin(); it != potentialNextStmts.end(); it++) {
					results.insert(*it);
					nextStmts = getAndMemoiseNextStar(isNext, *it);
					results.insert(nextStmts.begin(), nextStmts.end());
				}
				storeNextStar(stmt, results);
				return results;
			}
		}
	} else {
		if (donePreviousFlags[stmt]) {
			return previousStars[stmt];
		} else {
			oldestWhile = getOldestWhile(stmt);
			if (oldestWhile != 0) {
				whileChildren = pkb->getStmtsByStmt(PARENT_STAR, oldestWhile);
				whileChildren.insert(oldestWhile);
				results.insert(whileChildren.begin(), whileChildren.end());

				potentialNextStmts = pkb->getStmtsByStmt(oldestWhile, FOLLOWS);
				if (potentialNextStmts.empty() && oldestWhile != stmt) {
					// While is the start of a stmtLst 
					// Might be inside an if-else stmtLst
					potentialNextStmts = pkb->getStmtsByStmt(oldestWhile, PARENT);
				}
				for (it = potentialNextStmts.begin(); it != potentialNextStmts.end(); it++) {
					results.insert(*it);
					nextStmts = getAndMemoiseNextStar(isNext, *it);
					results.insert(nextStmts.begin(), nextStmts.end());
				}
				for (whileChildrenIt = whileChildren.begin(); whileChildrenIt != whileChildren.end(); whileChildrenIt++) {
					storePrevStar(*whileChildrenIt, results);
				}
				return results;
			} else {
				potentialNextStmts = pkb->getStmtsByStmt(stmt, NEXT); // Gets previous
				for (it = potentialNextStmts.begin(); it != potentialNextStmts.end(); it++) {
					results.insert(*it);
					nextStmts = getAndMemoiseNextStar(isNext, *it);
					results.insert(nextStmts.begin(), nextStmts.end());
				}
				storePrevStar(stmt, results);
				return results;
			}
		}
	}
}

void RelationshipPopulator::storeNextStar(StmtNumber stmt, std::set<StmtNumber> nextStar) {
	doneNextFlags[stmt] = true;
	nextStars[stmt] = nextStar;
}

void RelationshipPopulator::storePrevStar(StmtNumber stmt, std::set<StmtNumber> previousStar) {
	donePreviousFlags[stmt] = true;
	previousStars[stmt] = previousStar;
}

StmtNumber RelationshipPopulator::getOldestWhile(StmtNumber currentStmt) {
	StmtNumber oldestWhile = 0;
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> grandparents;
	StmtSetIterator grandparentsIt;
	if (pkb->getStmtTypeForStmt(currentStmt) == WHILE) oldestWhile = currentStmt;
	grandparents = pkb->getStmtsByStmt(currentStmt, PARENT_STAR);
	grandparentsIt = grandparents.begin();
	if (!grandparents.empty()) {
		while (grandparentsIt != grandparents.end()) {
			if (pkb->getStmtTypeForStmt(*grandparentsIt) == WHILE) {
				return *grandparentsIt;
			}
			grandparentsIt++;
		}
	}
	return oldestWhile;
}