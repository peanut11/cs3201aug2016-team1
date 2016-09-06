// Maintained by: Cheng Gee
//
// Accessed by:
// - Frontend
//
// Constructs:
// - VarTable
// - StmtTable

#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>


using namespace std;


#include "PKB.h"
#include "DesignExtractor.h"

int DesignExtractor() {
	return 0;
}
void DesignExtractor::process() {
	processFollowStar();
	processParentStar();
}
void DesignExtractor::processFollowStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();
	for (StmtNumber i = (size - 1); i >= size; i--) {


		std::vector<StmtNumber> followList = pkb->getStmtsByStmt(i, FOLLOWS);
		if (!followList.empty()) {
			StmtNumber add = followList.front();
			std::vector<StmtNumber> followStarList = pkb->getStmtsByStmt(add, FOLLOWSSTAR);
			for (StmtNumber j = 0; j < followStarList.size(); j++) {
				StmtNumber add2 = followStarList.at(j);
				pkb->getStmtsByStmt(add2, FOLLOWSSTAR);
			}

		}

	}
}

void DesignExtractor::processParentStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();
	for (StmtNumber i = 0; i < size; i++) {
		std::vector<StmtNumber> parentList = pkb->getStmtsByStmt(i, PARENT);
		if (!parentList.empty()) {
			StmtNumber add = parentList.front();
			std::vector<StmtNumber> parentStarList = pkb->getStmtsByStmt(add, PARENTSTAR);
			for (StmtNumber j = 0; j < parentStarList.size(); j++) {
				StmtNumber add2 = parentStarList.at(j);
				pkb->getStmtsByStmt(add2, PARENTSTAR);
			}
		}
	}
}
void DesignExtractor::updateStmtTable() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();
	std::vector<StmtNumber> whileList = pkb->getStmtsByType(WHILE);
	for (StmtNumber i = 0; i < whileList.size(); i++) {
		processWhileLoop(whileList.at(i));
	}

}
void DesignExtractor::processWhileLoop(StmtNumber i) {
	PKB* pkb = PKB::getInstance();
	std::vector<StmtNumber> followlist = pkb->getStmtsByStmt(i, FOLLOWS);
	StmtNumber followLine = followlist.front();
	std::vector<StmtNumber> followStar = pkb->getStmtsByStmt(i + 1, FOLLOWSSTAR);
	for (StmtNumber j = 0; j < followStar.size(); j++) {
		StmtNumber k = followStar.at(j);
		if (pkb->getStmtTypeForStmt(k) == WHILE) {
			processWhileLoop(k);
			
		}
		std::vector<StmtNumber> useList = pkb->getStmtsByStmt(k, USES);
		std::vector<StmtNumber> modifiesList = pkb->getStmtsByStmt(k, MODIFIES);
		for (StmtNumber l = 0; l < useList.size(); l++) {
			pkb->putStmtForStmt(i, USES, useList.at(l));
			pkb->putStmtForStmt(i, MODIFIES, modifiesList.at(l));
			VarName addToVartableUse = pkb->getVarName(useList.at(l));
			VarName addToVartableMod = pkb->getVarName(modifiesList.at(l));
			pkb->putVarForStmt(i, USES, addToVartableUse);
			pkb->putVarForStmt(i, MODIFIES, addToVartableMod);
			
		}
	}
}
