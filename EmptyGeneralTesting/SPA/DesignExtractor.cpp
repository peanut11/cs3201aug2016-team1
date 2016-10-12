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
    processFollowsStar();
    processParentStar();
    updateStmtTable();
}

void DesignExtractor::processCallsStar() {
    PKB* pkb = PKB::getInstance();
    size_t size = pkb->getProcTableSize();
    bool *visited = new bool[size];

    for (size_t i = 0; i < size; i++) {
        visited[i] = false;
    }

    for (StmtNumber stmt = 0; stmt < size; stmt++) {
        std::set<ProcIndex> callList = pkb->getProcsByProc(stmt, CALLS);

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
    std::set<ProcIndex> callList = pkb->getProcsByProc(index, CALLS);
	ProcName proc = pkb->getProcName(caller);
    for (StmtSetIterator callit = callList.begin(); callit != callList.end(); callit++) {
        ProcIndex call = *callit;
        if (!visited[call]) {
            dfs(call, visited, index);
        }

        ProcName addName = pkb->getProcName(call);
        pkb->putProcForProc(caller, CALLS_STAR, addName);
        std::set<ProcIndex> callStarList = pkb->getProcsByProc(call, CALLS_STAR);

        for (StmtSetIterator addInd = callStarList.begin(); addInd != callStarList.end(); addInd++) {
            ProcIndex addI = *addInd;
            ProcName addName = pkb->getProcName(addI);
            pkb->putProcForProc(caller, CALLS_STAR, addName);
        }
		std::set<StmtNumber> useList = pkb->getVarsByProc(call, USES);
		for (StmtSetIterator useInd = useList.begin(); useInd != useList.end(); useInd++) {
			VarIndex var = *useInd;
			VarName varName = pkb->getVarName(var);
			pkb->putVarForProc(proc, USES, varName);
			
		}
		std::set<StmtNumber> modList = pkb->getVarsByProc(call, MODIFIES);
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

    for (StmtNumber stmt = 1; stmt <= size; stmt++) {
        std::set<StmtNumber> followsList = pkb->getStmtsByStmt(stmt, FOLLOWS);

        if (!followsList.empty()) {
            StmtNumber add = *followsList.begin();
            pkb->putStmtForStmt(stmt, FOLLOWS_STAR, add);
            std::set<StmtNumber> followsStarList = pkb->getStmtsByStmt(add, FOLLOWS_STAR);

            for (StmtSetIterator fs = followsStarList.begin(); fs != followsStarList.end(); fs++) {
                StmtNumber add2 = *fs;
                pkb->putStmtForStmt(stmt, FOLLOWS_STAR, add2);
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
            pkb->putStmtForStmt(stmt, PARENT_STAR, add);
            std::set<StmtNumber> PARENT_STARList = pkb->getStmtsByStmt(add, PARENT_STAR);

            for (StmtSetIterator ps = PARENT_STARList.begin(); ps != PARENT_STARList.end(); ps++) {
                StmtNumber add2 = *ps;
                pkb->putStmtForStmt(stmt, PARENT_STAR, add2);
            }
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
        processLoopForUseAndModifies(*w);
    }
}

StmtNumber DesignExtractor::getWhileListSize() {
    PKB* pkb = PKB::getInstance();
    return pkb->getStmtsByType(WHILE).size();
}

void DesignExtractor::processLoopForUseAndModifies(StmtNumber w) { // For USES And Modifies
    PKB* pkb = PKB::getInstance();
    std::set<StmtNumber> followlist = pkb->getStmtsByStmt(w, FOLLOWED_BY);

    // StmtNumber followLine = *followlist.begin();
    std::set<StmtNumber> followStar = pkb->getStmtsByStmt(w + 1, FOLLOWED_BY_STAR);
    if ((pkb->getStmtTypeForStmt((w + 1)) == WHILE) || (pkb->getStmtTypeForStmt((w + 1)) == IF)) {
        processLoopForUseAndModifies((w + 1));
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
            processLoopForUseAndModifies(s);
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
