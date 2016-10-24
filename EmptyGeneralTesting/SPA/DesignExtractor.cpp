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
	processCallsStar();
	processCallsForUsesAndModifies(); // After processCallsStar
	processFollowsStar();
	updateStmtTable(); // Does processParentStar(*w)
}

void DesignExtractor::processCallsStar() {
	PKB* pkb = PKB::getInstance();
	size_t size = pkb->getProcTableSize();
	bool *visited = new bool[size];

	for (size_t i = 0; i < size; i++) {
		visited[i] = false;
	}

	for (StmtNumber stmt = 0; stmt < size; stmt++) {
		std::set<ProcIndex> callList = pkb->getProcsByProc(CALLS,stmt);

		if (!visited[stmt]) {
			visited[stmt] = true;

			for (StmtSetIterator callit = callList.begin(); callit != callList.end(); callit++) {
				ProcIndex call = *callit;

				if (!visited[call]) {
					dfs(call, visited, stmt);
				}
			}
		}
	}
}

void DesignExtractor::dfs(StmtNumber index, bool *visited, StmtNumber caller) {
	PKB* pkb = PKB::getInstance();
	visited[index] = true;
	ProcName indexName = pkb->getProcName(index);
	pkb->putProcForProc(caller, CALLS_STAR, indexName);

	std::set<ProcIndex> callList = pkb->getProcsByProc(CALLS,index);

	ProcName proc = pkb->getProcName(caller);
	for (StmtSetIterator callit = callList.begin(); callit != callList.end(); callit++) {
		ProcIndex call = *callit;
		if (!visited[call]) {
			dfs(call, visited, index);
		}

		ProcName addProcName = pkb->getProcName(call);
		pkb->putProcForProc(caller, CALLS_STAR, addProcName);
		std::set<ProcIndex> callStarList = pkb->getProcsByProc(CALLS_STAR,call);

		for (StmtSetIterator addInd = callStarList.begin(); addInd != callStarList.end(); addInd++) {
			ProcIndex addI = *addInd;
			ProcName addName = pkb->getProcName(addI);
			pkb->putProcForProc(caller, CALLS_STAR, addName);
		}
		std::set<StmtNumber> useList = pkb->getVarsByProc(call, USES_P);
		for (StmtSetIterator useInd = useList.begin(); useInd != useList.end(); useInd++) {
			VarIndex var = *useInd;
			VarName varName = pkb->getVarName(var);
			pkb->putVarForProc(proc, USES, varName);

		}
		std::set<StmtNumber> modList = pkb->getVarsByProc(call, MODIFIES_P);
		for (StmtSetIterator modInd = modList.begin(); modInd != modList.end(); modInd++) {
			VarIndex var = *modInd;
			VarName varName = pkb->getVarName(var);
			pkb->putVarForProc(proc, MODIFIES, varName);
		}
	}
}

void DesignExtractor::processFollowsStar() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();

	for (StmtNumber stmt = size; stmt >= 1; stmt--) {
		std::set<StmtNumber> followsList = pkb->getStmtsByStmt(FOLLOWS, stmt);

		if (!followsList.empty()) {
			StmtNumber add = *followsList.begin();
			pkb->putStmtForStmt(stmt, FOLLOWS_STAR, add);
			std::set<StmtNumber> followsStarList = pkb->getStmtsByStmt(FOLLOWS_STAR, add);

			for (StmtSetIterator fs = followsStarList.begin(); fs != followsStarList.end(); fs++) {
				StmtNumber add2 = *fs;
				pkb->putStmtForStmt(stmt, FOLLOWS_STAR, add2);
			}
		}
	}
}
/*
void DesignExtractor::processParentStar() {
PKB* pkb = PKB::getInstance();
StmtNumber size = pkb->getStmtTableSize();

for (StmtNumber stmt = 1; stmt <= size; stmt++) {
std::set<StmtNumber> parentList = pkb->getStmtsByStmt(stmt,PARENT);

if (!parentList.empty()) {
for (StmtSetIterator ind = parentList.begin(); ind != parentList.end(); ind++) {
StmtNumber add = *ind;
pkb->putStmtForStmt(stmt, PARENT_STAR, add);
std::set<StmtNumber> PARENT_STARList = pkb->getStmtsByStmt(add,PARENT_STAR);
for (StmtSetIterator ps = PARENT_STARList.begin(); ps != PARENT_STARList.end(); ps++) {
StmtNumber add2 = *ps;
pkb->putStmtForStmt(stmt, PARENT_STAR, add2);
}
}
}
}
}*/
void DesignExtractor::processParentStar(StmtNumber stmt) {
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> parentList = pkb->getStmtsByStmt(PARENT, stmt);
	for (StmtSetIterator ind = parentList.begin(); ind != parentList.end(); ind++) {
		StmtNumber add = *ind;
		pkb->putStmtForStmt(stmt, PARENT_STAR, add);
		std::set<StmtNumber> PARENT_List = pkb->getStmtsByStmt(PARENT, add);
		if (!PARENT_List.empty()) {
			processParentStar(add);
		}
		std::set<StmtNumber> PARENT_StarList = pkb->getStmtsByStmt(PARENT_STAR, add);
		for (StmtSetIterator ind2 = PARENT_StarList.begin(); ind2 != PARENT_StarList.end(); ind2++) {
			StmtNumber addStarInd = *ind2;
			pkb->putStmtForStmt(stmt, PARENT_STAR, addStarInd);
		}
	}

}
void DesignExtractor::updateStmtTable() {
	PKB* pkb = PKB::getInstance();
	StmtNumber size = pkb->getStmtTableSize();
	std::set<StmtNumber> whileList = pkb->getStmtsByType(WHILE);
	std::set<StmtNumber> ifList = pkb->getStmtsByType(IF);
	std::set<StmtNumber> mergeList;
	std::set_union(whileList.begin(), whileList.end(),
		ifList.begin(), ifList.end(),
		std::inserter(mergeList, mergeList.end()));

	for (StmtSetIterator w = mergeList.begin(); w != mergeList.end(); w++) {
		processLoopForUsesAndModifies(*w);
		processParentStar(*w);
	}
}

StmtNumber DesignExtractor::getWhileListSize() {
	PKB* pkb = PKB::getInstance();
	return pkb->getStmtsByType(WHILE).size();
}

void DesignExtractor::processCallsForUsesAndModifies() {
	PKB* pkb = PKB::getInstance();
	StmtSet calls = pkb->getStmtsByType(EntityType::CALL);

	for (StmtSetIterator call = calls.begin(); call != calls.end(); call++) {
		ProcIndex proc = pkb->getProcByCall(*call);

		std::set<VarIndex> usedVars = pkb->getVarsByProc(proc, USES_P);
		for (std::set<VarIndex>::const_iterator var = usedVars.begin(); var != usedVars.end(); var++) {
			VarName varName = pkb->getVarName(*var);
			pkb->putVarForStmt(*call, USES, varName);
		}

		std::set<VarIndex> modifiedVars = pkb->getVarsByProc(proc, MODIFIES_P);
		for (std::set<VarIndex>::const_iterator var = modifiedVars.begin(); var != modifiedVars.end(); var++) {
			VarName varName = pkb->getVarName(*var);
			pkb->putVarForStmt(*call, MODIFIES, varName);
		}
	}
}

void DesignExtractor::processLoopForUsesAndModifies(StmtNumber w) {
	PKB* pkb = PKB::getInstance();
	std::set<StmtNumber> followlist = pkb->getStmtsByStmt(FOLLOWS, w);

	// StmtNumber followLine = *followlist.begin();
	std::set<StmtNumber> followStar = pkb->getStmtsByStmt(FOLLOWS_STAR, w + 1);
	if ((pkb->getStmtTypeForStmt((w + 1)) == WHILE) || (pkb->getStmtTypeForStmt((w + 1)) == IF)) {
		processLoopForUsesAndModifies((w + 1));
	}

	std::set<StmtNumber> useList = pkb->getVarsByStmt((w + 1), USES);

	for (StmtSetIterator u = useList.begin(); u != useList.end(); u++) {
		VarName addToVartableUse = pkb->getVarName(*u);
		pkb->putVarForStmt(w, USES, addToVartableUse);
	}

	std::set<StmtNumber> modifiesList = pkb->getVarsByStmt((w + 1), MODIFIES);

	for (StmtSetIterator m = modifiesList.begin(); m != modifiesList.end(); m++) {
		VarName addToVartableMod = pkb->getVarName(*m);
		pkb->putVarForStmt(w, MODIFIES, addToVartableMod);
	}

	// From second line in while Loop
	for (StmtSetIterator f = followStar.begin(); f != followStar.end(); f++) {
		StmtNumber s = *f;

		if ((pkb->getStmtTypeForStmt(s) == WHILE) || (pkb->getStmtTypeForStmt(s) == IF)) {
			processLoopForUsesAndModifies(s);
		}

		std::set<StmtNumber> useList = pkb->getVarsByStmt(s, USES);

		for (StmtSetIterator u = useList.begin(); u != useList.end(); u++) {
			VarName addToVartableUse = pkb->getVarName(*u);
			pkb->putVarForStmt(w, USES, addToVartableUse);
		}

		std::set<StmtNumber> modifiesList = pkb->getVarsByStmt(s, MODIFIES);

        for (StmtSetIterator m = modifiesList.begin(); m != modifiesList.end(); m++) {
            VarName addToVartableMod = pkb->getVarName(*m);
            pkb->putVarForStmt(w, MODIFIES, addToVartableMod);
        }
	}

	EntityType checkType = pkb->getStmtTypeForStmt(w);
	if (checkType == IF) {
		StmtSet nextList = pkb->getStmtsByStmt(NEXT, w);
		StmtSetIterator nextIndex = nextList.begin();
		nextIndex++;
		int nextInd = *nextIndex;
		std::set<StmtNumber> useListForElse = pkb->getVarsByStmt((nextInd), USES);
		for (StmtSetIterator u = useListForElse.begin(); u != useListForElse.end(); u++) {
			VarName addToVartableUse = pkb->getVarName(*u);
			pkb->putVarForStmt(w, USES, addToVartableUse);
		}
		std::set<StmtNumber> modListForElse = pkb->getVarsByStmt((nextInd), MODIFIES);
		for (StmtSetIterator m = modListForElse.begin(); m != modListForElse.end(); m++) {
			VarName addToVartableMod = pkb->getVarName(*m);
			pkb->putVarForStmt(w, MODIFIES, addToVartableMod);
		}
		
		std::set<StmtNumber> followStarForElse = pkb->getStmtsByStmt(FOLLOWS_STAR, nextInd);
		for (StmtSetIterator f = followStarForElse.begin(); f != followStarForElse.end(); f++) {
			StmtNumber s = *f;

			if ((pkb->getStmtTypeForStmt(s) == WHILE) || (pkb->getStmtTypeForStmt(s) == IF)) {
				processLoopForUsesAndModifies(s);
			}

			std::set<StmtNumber> useList = pkb->getVarsByStmt(s, USES);

			for (StmtSetIterator u = useList.begin(); u != useList.end(); u++) {
				VarName addToVartableUse = pkb->getVarName(*u);
				pkb->putVarForStmt(w, USES, addToVartableUse);
			}

			std::set<StmtNumber> modifiesList = pkb->getVarsByStmt(s, MODIFIES);

			for (StmtSetIterator m = modifiesList.begin(); m != modifiesList.end(); m++) {
				VarName addToVartableMod = pkb->getVarName(*m);
				pkb->putVarForStmt(w, MODIFIES, addToVartableMod);
			}

		}
	}
}
