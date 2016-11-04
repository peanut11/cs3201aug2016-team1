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
		hasNextStars.push_back(false);
		hasPrevStars.push_back(false);
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
void RelationshipPopulator::storeNextStar(StmtNumber stmt, std::set<StmtNumber> nextStar) {
	hasNextStars[stmt] = true;
	nextStars[stmt].insert(nextStar.begin(), nextStar.end());
}

void RelationshipPopulator::storePrevStar(StmtNumber stmt, std::set<StmtNumber> previousStar) {
	hasPrevStars[stmt] = true;
	previousStars[stmt].insert(previousStar.begin(), previousStar.end());
}

StmtNumber RelationshipPopulator::getOldestWhile(StmtNumber currentStmt) {
	StmtNumber oldestWhile = 0;
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> grandparents;
	StmtSetIterator grandparentsIt;
	//if (pkb->getStmtTypeForStmt(currentStmt) == WHILE) oldestWhile = currentStmt;
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


std::set<StmtNumber> RelationshipPopulator::getNextStar(StmtNumber startStmt, StmtNumber endStmt)
{
	std::set<StmtNumber> results;
	PKB* pkb = PKB::getInstance();
	bool isNext;
	StmtNumber oldestWhile;

	if (startStmt != 0 && endStmt != 0) {
		// Check if same procedure
		if (pkb->getProcByStmt(startStmt) != pkb->getProcByStmt(endStmt)) {
			return results;
		} else if (hasNextStars[startStmt]) {
			if (nextStars[startStmt].find(endStmt) != nextStars[startStmt].end()) {
				return nextStars[startStmt];
			}
		} else if (hasPrevStars[endStmt]) {
			if (previousStars[endStmt].find(startStmt) != previousStars[endStmt].end()) {
				return previousStars[endStmt];
			}
		}
	}

	if (startStmt == 0 && endStmt == 0) { // Invalid 
		return results;
	} else if (startStmt != 0) {
		isNext = true;
	} else { // startStmt == 0 && endStmt != 0
		isNext = false;
	}

	if (isNext) {
		oldestWhile = getOldestWhile(startStmt); // oldestWhile is not inclusive of startStmt

		if (oldestWhile != 0) { // startStmt is a child of WHILE loop
			return getNextStar(oldestWhile, endStmt);
			// Don't need to store NextStar of children in WHILE 
		} else if (pkb->getStmtTypeForStmt(startStmt) == WHILE) {
			if (hasNextStars[startStmt]) {
				results = nextStars[startStmt];

				if (endStmt != 0 && results.find(endStmt) != results.end()) {
					return results;
				}

				StmtNumber follower = getFollowsOfWhile(startStmt);
				if (follower != 0) {
					results = getNextStar(getFollowsOfWhile(startStmt), endStmt);
					results.insert(follower);
					storeNextStar(startStmt, results);
				}


			} else {
				results = pkb->getStmtsByStmt(PARENT_STAR, startStmt);
				results.insert(startStmt);
				storeNextStar(startStmt, results);

				if (endStmt != 0 && results.find(endStmt) != results.end()) {
					return results;
				}

				StmtNumber follower = getFollowsOfWhile(startStmt);
				if (follower != 0) {
					results = getNextStar(getFollowsOfWhile(startStmt), endStmt);
					results.insert(follower);
					storeNextStar(startStmt, results);
				}
			}
		} else {
			if (hasNextStars[startStmt]) {
				results = nextStars[startStmt];

				if (endStmt != 0 && results.find(endStmt) != results.end()) {
					return results;
				}

				if (!nextStars[startStmt].empty()) {
					results = getNextStar(getLastOfMemoiseNextStar(startStmt), endStmt);
					storeNextStar(startStmt, results);
				}

			} else {
				results = pkb->getStmtsByStmt(NEXT, startStmt);
				storeNextStar(startStmt, results);
				StmtSetIterator it = results.begin();
				while (it != results.end()) {
					storeNextStar(startStmt, getNextStar(*it, endStmt));
					if (endStmt != 0 && *it == endStmt) {
						return nextStars[startStmt];
					}
					it++;
				}
			}
		}
		return nextStars[startStmt];
	} else { // Previous*
		oldestWhile = getOldestWhile(endStmt);
		if (oldestWhile != 0) {
			return getNextStar(StmtNumber(0), oldestWhile);

		} else if (pkb->getStmtTypeForStmt(endStmt) == WHILE) {
			if (hasPrevStars[endStmt]) {
				results = previousStars[endStmt];

				results = getNextStar(StmtNumber(0), getLeaderOfWhile(endStmt));
				storePrevStar(endStmt, results);
			} else {
				results = pkb->getStmtsByStmt(PARENT_STAR, endStmt);
				results.insert(endStmt);
				storePrevStar(endStmt, results);

				StmtNumber leader = getLeaderOfWhile(endStmt);
				if (leader != 0) {
					results = getNextStar(StmtNumber(0), leader);
					results.insert(leader);
					storePrevStar(endStmt, results);
				}
			}
		} else {
			if (hasPrevStars[endStmt]) {
				if (!previousStars[endStmt].empty()) {
					results = getNextStar(StmtNumber(0), getFirstOfMemoisePrevStar(endStmt));
					storePrevStar(endStmt, results);
				}

			} else {
				results = pkb->getStmtsByStmt(endStmt, NEXT);
				storePrevStar(endStmt, results);
				StmtSetIterator it = results.begin();
				while (it != results.end()) {
					storePrevStar(endStmt, getNextStar(StmtNumber(0), *it));
					it++;
				}
			}
		}
		return previousStars[endStmt];
	}
}

StmtNumber RelationshipPopulator::getFollowsOfWhile(StmtNumber stmt) {
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> follows = pkb->getStmtsByStmt(FOLLOWS, stmt);

	StmtNumber currentStmt = stmt;
	bool isContinue = true;
	while (follows.empty() && isContinue) {
		// While is the end of a stmtLst
		// Might be at the end of many nested if-else stmtLst
		StmtSet parentSet = pkb->getStmtsByStmt(currentStmt, PARENT);
		if (!parentSet.empty()) {
			StmtNumber parent = *parentSet.begin();
			if (pkb->getStmtTypeForStmt(parent) == IF) {
				StmtSet followSet = pkb->getStmtsByStmt(FOLLOWS, parent);
				follows = followSet;
				currentStmt = parent;
			} else {
				isContinue = false;
			}
		} else {
			isContinue = false;
		}
	}
	if (!follows.empty()) {
		return *follows.begin();
	} else {
		return StmtNumber(0);
	}
	
}

StmtNumber RelationshipPopulator::getLastOfMemoiseNextStar(StmtNumber stmt) {
	if (!nextStars[stmt].empty()) {
		return *nextStars[stmt].rbegin();
	} else return 0;
}

StmtNumber RelationshipPopulator::getLeaderOfWhile(StmtNumber stmt) {
	PKB* pkb = PKB::getInstance();
	StmtSet leaders = pkb->getStmtsByStmt(stmt, FOLLOWS);

	if (leaders.empty()) {
		// While is the start of a stmtLst 
		// Might be inside an if-else stmtLst
		StmtSet parentSet = pkb->getStmtsByStmt(stmt, PARENT);
		if (!parentSet.empty()) {
			StmtNumber parent = *parentSet.begin();
			if (pkb->getStmtTypeForStmt(parent) == IF) {
				leaders.insert(parent);
			} else {
				// Parent is a while -> oldestWhile is not oldest
				throw std::runtime_error("");
			}
		} else {
			// While is the start of a procedure
		}
	}

	if (!leaders.empty()) {
		return *leaders.begin();
	} else {
		return StmtNumber(0);
	}
}

StmtNumber RelationshipPopulator::getFirstOfMemoisePrevStar(StmtNumber stmt) {
	if (!previousStars[stmt].empty()) {
		return *previousStars[stmt].begin();
	} else return 0;
}