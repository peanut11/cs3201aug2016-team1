// Maintained by: Kai Lin
//
// Facade of:
// - AssignmentTrees
// - StmtTable
// - VarTable

#include "PKB.h"

const std::runtime_error PKB::ERROR = std::runtime_error("");

bool PKB::contains(std::vector<VarOrStmt> vec, VarOrStmt item) {
	for (std::vector<VarOrStmt>::const_iterator it = vec.begin(); it < vec.end(); it++) {
		if (*it == item) {
			return true;
		}
	}

	return false;
}

PKB* PKB::getInstance() {
	if (theOne == nullptr) {
		theOne = new PKB();
	}

	return theOne;
}


bool PKB::is(StmtNumber stmt, RelationshipType rel, VarOrStmt item) {
	return contains(stmtTable[stmt][rel], item);
}

bool PKB::is(RelationshipType rel, StmtNumber stmtA, StmtNumber stmtB) {
	return contains(stmtTable[stmtA][rel], stmtB);
}

std::vector<Constant> PKB::getAllConstantValues() {
	return constants;
}

std::vector<VarName> PKB::getAllVarNames() {
	return refTable;
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

std::vector<StmtNumber> PKB::getStmts(StmtNumber stmt, RelationshipType stmtRel) {
	if (stmtRel == MODIFIES || stmtRel == USES) {
		throw ERROR;
	}

	return stmtTable[stmt][stmtRel];
}

std::vector<StmtNumber> PKB::getStmts(EntityType stmtType) {
	std::vector<StmtNumber> stmts;

	for (StmtNumber i = 0; i < stmtTable.size(); i++) {
		if (stmtTypeTable[i] == stmtType) {
			stmts.push_back(i);
		}
	}

	return std::vector<StmtNumber>();
}

VarIndex PKB::getVarIndex(VarName varName) {
	RefMap::const_iterator it = refMap.find(varName);
	VarIndex varIndex;
	
	if (it == refMap.end()) {
		varIndex = varTable.size();
		VarRow newRow = VarRow();
		varTable.push_back(newRow);
		refTable.push_back(varName);
		refMap[varName] = varIndex;
	} else {
		varIndex = it->second;
	}

	return varIndex;
}

VarName PKB::getVarName(VarIndex varIndex) {
	if (varIndex >= refTable.size()) {
		throw ERROR;
	}

	VarName varName = refTable[varIndex];
	return varName;
}

std::vector<VarIndex> PKB::getVars(StmtNumber stmt, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw ERROR;
	}

	return stmtTable[stmt][modifiesOrUses];
}

bool PKB::putVar(StmtNumber dest, RelationshipType rel, VarIndex varIndex) {
	if (dest > stmtTable.size()) {
		throw ERROR;
	}

	if (rel != MODIFIES && rel != USES) {
		throw ERROR;
	}

	const int prevSize = stmtTable[dest][rel].size();
	stmtTable[dest][rel].push_back(varIndex);

	return (prevSize + 1 == stmtTable[dest][rel].size());
}

bool PKB::putStmt(StmtNumber dest, RelationshipType rel, StmtNumber stmt) {
	if (dest > stmtTable.size()) {
		throw ERROR;
	}

	if (rel == MODIFIES || rel == USES) {
		throw ERROR;
	}

	if (rel == FOLLOWS || rel == PARENT) {
		const int OFFSET = 1;
		stmtTable[dest][rel + OFFSET].push_back(stmt);
	}

	const int prevSize = stmtTable[dest][rel].size();
	stmtTable[dest][rel].push_back(stmt);

	return (prevSize + 1 == stmtTable[dest][rel].size());
}

bool PKB::putAssign(StmtNumber dest, AssignTree tree) {
	while (dest > assignTrees.size()) {
		assignTrees.push_back(AssignTree());
	}

	assignTrees.push_back(tree);

	return (dest + 1 == assignTrees.size());
}
