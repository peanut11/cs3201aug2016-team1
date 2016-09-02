// Maintained by: Kai Lin
//
// Facade of:
// - AssignmentTrees
// - StmtTable
// - VarTable

#include "PKB.h"

const std::runtime_error PKB::ERROR = std::runtime_error("");

PKB* PKB::getInstance() {
	if (theOne == nullptr) {
		theOne = new PKB();
	}
	return theOne;
}

std::vector<CONST> PKB::getAllConstantValues() {
	return constants;
}

std::vector<VAR_NAME> PKB::getAllVarNames() {
	std::vector<VAR_NAME> varNames;

	for (REF_TABLE::iterator it = refTable.begin(); it != refTable.end(); it++) {
		std::string key = it->first;
		varNames.push_back(key);
	}

	return varNames;
}

AssignTree PKB::getAssign(STMT_NUM stmt) {
	if (stmtTypeTable[stmt] != EntityType::ASSIGN) {
		throw ERROR;
	}

	return assignTrees[stmt];
}

std::vector<STMT_NUM> PKB::getStmts(VAR_INDEX var, RelationshipType rel) {
	return varTable[var][rel];
}

std::vector<VAR_INDEX> PKB::getVars(STMT_NUM stmt, RelationshipType relIsMU) {
	if (relIsMU != MODIFIES && relIsMU != USES) {
		throw ERROR;
	}

	return stmtTable[stmt][relIsMU];
}

std::vector<STMT_NUM> PKB::getStmts(RelationshipType relNotMU, STMT_NUM stmt) {
	if (relNotMU == MODIFIES || relNotMU == USES) {
		throw ERROR;
	}

	return stmtTable[stmt][relNotMU];
}

std::vector<STMT_NUM> PKB::getStmts(EntityType stmtType) {
	std::vector<STMT_NUM> stmts;

	for (STMT_NUM i = 0; i < stmtTable.size(); i++) {
		if (stmtTypeTable[i] == stmtType) {
			stmts.push_back(i);
		}
	}

	return std::vector<STMT_NUM>();
}

bool PKB::putVar(STMT_NUM dest, RelationshipType rel, VAR_INDEX var) {
	if (dest > stmtTable.size()) {
		throw ERROR;
	}

	if (rel != MODIFIES && rel != USES) {
		throw ERROR;
	}

	const int prevSize = stmtTable[dest][rel].size();
	stmtTable[dest][rel].push_back(var);

	return (prevSize + 1 == stmtTable[dest][rel].size());
}

bool PKB::putStmt(STMT_NUM dest, RelationshipType rel, STMT_NUM stmt) {
	if (dest > stmtTable.size()) {
		throw ERROR;
	}

	if (rel == MODIFIES || rel == USES) {
		throw ERROR;
	}

	const int prevSize = stmtTable[dest][rel].size();
	stmtTable[dest][rel].push_back(stmt);

	return (prevSize + 1 == stmtTable[dest][rel].size());
}

bool PKB::putAssign(STMT_NUM dest, AssignTree tree) {
	while (dest > assignTrees.size()) {
		assignTrees.push_back(AssignTree());
	}

	assignTrees.push_back(tree);

	return (dest + 1 == assignTrees.size());
}
