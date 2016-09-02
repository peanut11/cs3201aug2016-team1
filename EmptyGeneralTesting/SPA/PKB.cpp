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

std::vector<Constant> PKB::getAllConstantValues() {
	return constants;
}

std::vector<VarName> PKB::getAllVarNames() {
	std::vector<VarName> varNames;

	for (RefTable::const_iterator it = refTable.begin(); it != refTable.end(); it++) {
		std::string key = it->first;
		varNames.push_back(key);
	}

	return varNames;
}

AssignTree PKB::getAssign(StmtNumber stmt) {
	if (stmtTypeTable[stmt] != EntityType::ASSIGN) {
		throw ERROR;
	}

	return assignTrees[stmt];
}

std::vector<StmtNumber> PKB::getStmts(RelationshipType rel, VarIndex varIndex) {
	return varTable[varIndex][rel];
}

std::vector<StmtNumber> PKB::getStmts(StmtNumber stmt, RelationshipType relNotMU) {
	if (relNotMU == MODIFIES || relNotMU == USES) {
		throw ERROR;
	}

	return stmtStmtTable[stmt][relNotMU];
}

std::vector<StmtNumber> PKB::getStmts(EntityType stmtType) {
	std::vector<StmtNumber> stmts;

	for (StmtNumber i = 0; i < stmtStmtTable.size(); i++) {
		if (stmtTypeTable[i] == stmtType) {
			stmts.push_back(i);
		}
	}

	return std::vector<StmtNumber>();
}

VarIndex PKB::getVarIndex(VarName varName) {
	RefTable::const_iterator it = refTable.find(varName);
	int varIndex;
	
	if (it == refTable.end()) {
		varIndex = varTable.size();
		VarRow newRow = VarRow();
		varTable.push_back(newRow);
		refTable[varName] = varIndex;
	} else {
		varIndex = it->second;
	}

	return varIndex;
}

std::vector<VarIndex> PKB::getVars(StmtNumber stmt, RelationshipType relIsMU) {
	if (relIsMU != MODIFIES && relIsMU != USES) {
		throw ERROR;
	}

	return stmtVarTable[stmt][relIsMU];
}

bool PKB::putVar(StmtNumber dest, RelationshipType rel, VarIndex varIndex) {
	if (dest > stmtVarTable.size()) {
		throw ERROR;
	}

	if (rel != MODIFIES && rel != USES) {
		throw ERROR;
	}

	const int prevSize = stmtVarTable[dest][rel].size();
	stmtVarTable[dest][rel].push_back(varIndex);

	return (prevSize + 1 == stmtVarTable[dest][rel].size());
}

bool PKB::putStmt(StmtNumber dest, RelationshipType rel, StmtNumber stmt) {
	if (dest > stmtStmtTable.size()) {
		throw ERROR;
	}

	if (rel == MODIFIES || rel == USES) {
		throw ERROR;
	}

	const int prevSize = stmtStmtTable[dest][rel].size();
	stmtStmtTable[dest][rel].push_back(stmt);

	return (prevSize + 1 == stmtStmtTable[dest][rel].size());
}

bool PKB::putAssign(StmtNumber dest, AssignTree tree) {
	while (dest > assignTrees.size()) {
		assignTrees.push_back(AssignTree());
	}

	assignTrees.push_back(tree);

	return (dest + 1 == assignTrees.size());
}
