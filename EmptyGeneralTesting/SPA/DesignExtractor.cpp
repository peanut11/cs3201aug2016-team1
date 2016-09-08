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
	processFollowStar();
	processParentStar();
}

void DesignExtractor::processFollowStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();

	for (StmtNumber i = (size - 1); i >= size; i--) {
		std::set<StmtNumber> followList = pkb->getStmtsByStmt(i, FOLLOWS);

		if (!followList.empty()) {
			StmtNumber add = *followList.begin();
			std::set<StmtNumber> followStarList = pkb->getStmtsByStmt(add, FOLLOWSSTAR);

			for (StmtSetIterator fs = followStarList.begin(); fs != followStarList.end(); fs++) {
				StmtNumber add2 = *fs;
				pkb->getStmtsByStmt(add2, FOLLOWSSTAR);
			}
		}
	}
}

void DesignExtractor::processParentStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();

	for (StmtNumber i = 0; i < size; i++) {
		std::set<StmtNumber> parentList = pkb->getStmtsByStmt(i, PARENT);

		if (!parentList.empty()) {
			StmtNumber add = *parentList.begin();
			std::set<StmtNumber> parentStarList = pkb->getStmtsByStmt(add, PARENTSTAR);

			for (StmtSetIterator ps = parentStarList.begin(); ps != parentStarList.end(); ps++) {
				StmtNumber add2 = *ps;
				pkb->getStmtsByStmt(add2, PARENTSTAR);
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
	std::set<StmtNumber> followlist = pkb->getStmtsByStmt(w, FOLLOWS);
	StmtNumber followLine = *followlist.begin();
	std::set<StmtNumber> followStar = pkb->getStmtsByStmt(w + 1, FOLLOWSSTAR);

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
