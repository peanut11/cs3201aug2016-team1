#pragma once
#include "AffectsCalculator.h"
#include "PKB.h"

AffectsCalculator * AffectsCalculator::instance = nullptr;
PKB * pkb = PKB::getInstance();

AffectsCalculator::AffectsCalculator() {
	stmtCount = pkb->getStmtTableSize();
}

AffectsCalculator* AffectsCalculator::getInstance() {
	if (instance == nullptr) {
		instance = new AffectsCalculator();
	}

	return instance;
}

void AffectsCalculator::clear() {
	if (instance != nullptr) {
		delete instance;
	}

	instance = new AffectsCalculator();
}

bool AffectsCalculator::isAffects(StmtNumber x, StmtNumber y, bool isStar) {
	if (pkb->getStmtTypeForStmt(x) != ASSIGN || pkb->getStmtTypeForStmt(y) != ASSIGN) {
		return false;
	}

	if (pkb->getProcByStmt(x) != pkb->getProcByStmt(y)) {
		return false;
	}

	VarIndex v = *((pkb->getVarsByStmt(x, MODIFIES)).begin());

	if (!isStar && !pkb->is(USES, y, v)) {
		return false;
	}

	dest = y;
	for (int i = 0; i < 2; i++) {
		visited[i] = std::vector<std::set<VarIndex> >(stmtCount + 1);
	}
	return dfs(true, x, v, isStar);
}

StmtSet AffectsCalculator::getAffected(StmtNumber x, bool isStar) {
	if (pkb->getStmtTypeForStmt(x) != ASSIGN) {
		return StmtSet();
	}

	VarIndex v = *((pkb->getVarsByStmt(x, MODIFIES)).begin());

	results.clear();
	for (int i = 0; i < 2; i++) {
		visited[i] = std::vector<std::set<VarIndex> >(stmtCount + 1);
	}
	dfsAffected(true, x, v, isStar);
	return results;
}

StmtSet AffectsCalculator::getAffectors(StmtNumber x, bool isStar) {
	if (pkb->getStmtTypeForStmt(x) != ASSIGN) {
		return StmtSet();
	}

	results.clear();
	for (int i = 0; i < 2; i++) {
		visited[i] = std::vector<std::set<VarIndex> >(stmtCount + 1);
	}
	for (VarIndex v : pkb->getVarsByStmt(x, USES)) {
		dfsAffectors(true, x, v, isStar);
	}
	return results;
}

bool AffectsCalculator::dfs(bool isBegin, StmtNumber x, VarIndex v, bool isStar) {
	visited[isBegin][x].insert(v);
	EntityType type = pkb->getStmtTypeForStmt(x);

	if (!isBegin && x == dest && pkb->is(USES, x, v)) {
		return true;
	}

	if (!isBegin && (type == ASSIGN || type == CALL) && pkb->is(MODIFIES, x, v)) {
		return false;
	}

	bool hasNew = isStar && !isBegin && type == ASSIGN && pkb->is(USES, x, v);
	VarIndex newVar = hasNew ? *((pkb->getVarsByStmt(x ,MODIFIES)).begin()) : 0;

	for (StmtNumber s : pkb->getStmtsByStmt(NEXT, x)) {
		if (visited[isBegin][s].find(v) == visited[isBegin][s].end()) {
			if (dfs(false, s, v, isStar)) {
				return true;
			}
		}

		if (hasNew && visited[isBegin][s].find(newVar) == visited[isBegin][s].end()) {
			if (dfs(false, s, newVar, isStar)) {
				return true;
			}
		}
	}

	return false;
}

void AffectsCalculator::dfsAffected(bool isBegin, StmtNumber x, VarIndex v, bool isStar) {
	visited[isBegin][x].insert(v);
	EntityType type = pkb->getStmtTypeForStmt(x);

	bool affected = !isBegin && type == ASSIGN && pkb->is(USES, x, v);
	if (affected) {
		results.insert(x);
	}

	if (!isBegin && (type == ASSIGN || type == CALL) && pkb->is(MODIFIES, x, v)) {
		if (affected && isStar) {
			dfsAffected(true, x, v, isStar);
		}
		return;
	}

	bool hasNew = isStar && affected;
	VarIndex newVar = hasNew ? *((pkb->getVarsByStmt(x, MODIFIES)).begin()) : 0;

	for (StmtNumber s : pkb->getStmtsByStmt(NEXT, x)) {
		if (visited[isBegin][s].find(v) == visited[isBegin][s].end()) {
			dfsAffected(false, s, v, isStar);
		}

		if (hasNew && visited[isBegin][s].find(newVar) == visited[isBegin][s].end()) {
			dfsAffected(false, s, newVar, isStar);
		}
	}
}

void AffectsCalculator::dfsAffectors(bool isEnd, StmtNumber x, VarIndex v, bool isStar) {
	visited[isEnd][x].insert(v);
	EntityType type = pkb->getStmtTypeForStmt(x);
	bool mod = pkb->is(MODIFIES, x, v);

	if (type == CALL && mod) {
		return;
	}

	bool affector = !isEnd && type == ASSIGN && mod;

	if (affector) {
		results.insert(x);
		if (!isStar) {
			return;
		}
	}

	ProcIndex proc = pkb->getProcByStmt(x);

	for (StmtNumber s : pkb->getStmtsByStmt(x, NEXT)) { // get PREVIOUS of x
		if (affector && isStar) {
			for (VarIndex newVar : pkb->getVarsByStmt(x, USES)) {
				if (pkb->is(MODIFIES_P, proc, newVar) && 
					visited[isEnd][s].find(newVar) == visited[isEnd][s].end()) {
					dfsAffectors(false, s, newVar, isStar);
				}
			}
		}
		else {
			if (visited[isEnd][s].find(v) == visited[isEnd][s].end()) {
				dfsAffectors(false, s, v, isStar);
			}
		}
	}
}