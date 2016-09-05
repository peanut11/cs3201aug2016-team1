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
StmtRow DesignExtractor::processFollowStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtsTableSize;
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

StmtRow DesignExtractor::processParentStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtsTableSize;
	for (StmtNumber i = 0; i < size; i++) {
		PKB* pkb = PKB::getInstance();
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


