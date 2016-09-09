// Maintained by: Cheng Gee
//
// Accessed by:
// - Frontend
//
// Constructs:
// - VarTable
// - StmtTable

#pragma once

#include "DesignExtractor.h"



DesignExtractor::DesignExtractor() {}

void DesignExtractor::process() {
	processFollowedByStar();
	processParentStar();
	processFollowsStar();
	processParentOfStar();
	updateStmtTable();
}

void DesignExtractor::processFollowedByStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();

	for (StmtNumber stmt = (size - 1); stmt > 0; stmt--) {
		std::set<StmtNumber> followedByList = pkb->getStmtsByStmt(stmt, FOLLOWED_BY);

		if (!followedByList.empty()) {
			StmtNumber follower = *followedByList.begin();
			std::set<StmtNumber> followedByStarList = pkb->getStmtsByStmt(follower, FOLLOWED_BY_STAR);
			pkb->putStmtForStmt(stmt, FOLLOWED_BY_STAR, follower);
			for (StmtSetIterator fbs = followedByStarList.begin(); fbs != followedByStarList.end(); fbs++) {
				StmtNumber followerFollower = *fbs;
				pkb->putStmtForStmt(stmt, FOLLOWED_BY_STAR, followerFollower);
			}
		}
	}
}
void DesignExtractor::processParentOfStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();
	for (StmtNumber stmt = (size - 1); stmt > 0; stmt--) {
		std::set<StmtNumber> parentOfList = pkb->getStmtsByStmt(stmt, PARENT_OF);
		
			for (StmtSetIterator parentOfpos = parentOfList.begin(); parentOfpos!=parentOfList.end();parentOfpos++){

			std::set<StmtNumber> parentOfStarList = pkb->getStmtsByStmt(*parentOfpos, PARENT_OF_STAR);
			pkb->putStmtForStmt(stmt, PARENT_OF_STAR, *parentOfpos);
			for (StmtSetIterator pos = parentOfStarList.begin(); pos != parentOfStarList.end(); pos++) {
				StmtNumber followerFollower = *pos;
				pkb->putStmtForStmt(stmt, PARENT_OF_STAR, followerFollower);
			}
		}
	}
}
void DesignExtractor::processFollowsStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();
	for (StmtNumber stmt = 1; stmt <= size; stmt++) {
		std::set<StmtNumber> followsList = pkb->getStmtsByStmt(stmt, FOLLOWS);
		if (!followsList.empty()) {
			StmtNumber add = *followsList.begin();
			pkb->putStmtForStmt(stmt, FOLLOWS_STAR, add);
			std::set<StmtNumber> followsStarList = pkb->getStmtsByStmt(add, FOLLOWS_STAR);

			for (StmtSetIterator fs = followsStarList.begin(); fs != followsStarList.end(); fs++) {
				StmtNumber add2 = *fs;
				pkb->putStmtForStmt(stmt, PARENTSTAR, add2);
			}
		}
	}
}

void DesignExtractor::processParentStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();

	for (StmtNumber stmt = 1; stmt <= size; stmt++) {
		std::set<StmtNumber> parentList = pkb->getStmtsByStmt(stmt, PARENT);
		
		if (!parentList.empty()) {
			
			StmtNumber add = *parentList.begin();
			pkb->putStmtForStmt(stmt, PARENTSTAR, add);
			std::set<StmtNumber> parentStarList = pkb->getStmtsByStmt(add, PARENTSTAR);

			for (StmtSetIterator ps = parentStarList.begin(); ps != parentStarList.end(); ps++) {
				StmtNumber add2 = *ps;
				pkb->putStmtForStmt(stmt, PARENTSTAR,add2);
			}
		}
	}
}

void DesignExtractor::updateStmtTable() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();
	std::set<StmtNumber> whileList = pkb->getStmtsByType(WHILE);

	for (StmtSetIterator w = whileList.begin(); w != whileList.end(); w++) {
		processWhileLoop(*w);
	}
}

void DesignExtractor::processWhileLoop(StmtNumber w) {
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> followlist = pkb->getStmtsByStmt(w, FOLLOWED_BY);
	StmtNumber followLine = *followlist.begin();
	std::set<StmtNumber> followStar = pkb->getStmtsByStmt(w + 1, FOLLOWED_BY_STAR);

	for (StmtSetIterator f = followStar.begin(); f != followStar.end(); f++) {
		StmtNumber s = *f;
		if (pkb->getStmtTypeForStmt(s) == WHILE) {
			processWhileLoop(s);
		}

		std::set<StmtNumber> useList = pkb->getStmtsByStmt(s, USES);
		for (StmtSetIterator u = useList.begin(); u != useList.end(); u++) {
			pkb->putStmtForStmt(w, USES, *u);
			VarName addToVartableUse = pkb->getVarName(*u);
			pkb->putVarForStmt(w, USES, addToVartableUse);
		}

		std::set<StmtNumber> modifiesList = pkb->getStmtsByStmt(s, MODIFIES);
		for (StmtSetIterator m = modifiesList.begin(); m != modifiesList.end(); m++) {
			pkb->putStmtForStmt(w, MODIFIES, *m);
			VarName addToVartableMod = pkb->getVarName(*m);
			pkb->putVarForStmt(w, MODIFIES, addToVartableMod);
		}
	}
}
