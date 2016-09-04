// Maintained by: Kai Lin
//
// Facade of:
// - AssignmentTrees
// - StmtTable
// - VarTable

#include "PKB.h"

const std::runtime_error PKB::ERROR = std::runtime_error("");

PKB* PKB::theOne = nullptr;

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

PKB::PKB() {
	assignTrees = std::vector<AssignTree>();
	constants = std::vector<Constant>();
	refMap = RefMap();
	refTable = std::vector<VarName>();
	stmtTable = std::vector<StmtRow>();
	stmtTypeTable = std::vector<EntityType>();
	varTable = std::vector<VarRow>();
}

bool PKB::is(StmtNumber stmt, RelationshipType rel, VarOrStmt item) {
	return contains(stmtTable[stmt][rel], item);
}

bool PKB::is(RelationshipType rel, StmtNumber stmtA, StmtNumber stmtB) {
	return contains(stmtTable[stmtA][rel], stmtB);
}

bool PKB::isVarExist(VarName varName) {
	return (refMap.find(varName) != refMap.end());
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

std::vector<StmtNumber> PKB::getStmtsByVar(RelationshipType rel, VarIndex varIndex) {
	return varTable[varIndex][rel];
}

std::vector<StmtNumber> PKB::getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel) {
	if (stmtRel == MODIFIES || stmtRel == USES) {
		throw ERROR;
	}

	return stmtTable[stmt][stmtRel];
}

std::vector<StmtNumber> PKB::getStmtsByType(EntityType stmtType) {
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
		varTable.push_back(VarRow());
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

std::vector<VarIndex> PKB::getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw ERROR;
	}

	return stmtTable[stmt][modifiesOrUses];
}

bool PKB::putVarForStmt(StmtNumber stmt, RelationshipType rel, VarIndex varIndex) {
	bool success; 
	int prevSize;

	if (rel != MODIFIES && rel != USES) {
		throw ERROR;
	}
	
	while (stmt >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}

	while (varIndex >= varTable.size()) {
		varTable.push_back(VarRow());
	}
	
	prevSize = stmtTable[stmt][rel].size();
	stmtTable[stmt][rel].push_back(varIndex);
	success = (prevSize + 1 == stmtTable[stmt][rel].size());

	prevSize = varTable[varIndex][rel].size();
	varTable[varIndex][rel].push_back(stmt);
	success = (prevSize + 1 == varTable[varIndex][rel].size()) && success;

	return success;
}

bool PKB::putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB) {
	bool success;

	if (rel == MODIFIES || rel == USES) {
		throw ERROR;
	}

	while (stmtB >= stmtTable.size() || stmtA >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}

	int prevSize = stmtTable[stmtA][rel].size();

	prevSize = stmtTable[stmtA][rel].size();
	stmtTable[stmtA][rel].push_back(stmtB);
	success = (prevSize + 1 == stmtTable[stmtA][rel].size());

	if (rel == FOLLOWS || rel == PARENT || rel == FOLLOWED_BY || rel == PARENT_OF) {
		const int OFFSET = 1;
		int supplementaryRel;

		if (rel == FOLLOWS || rel == PARENT) {
			supplementaryRel = rel + OFFSET;
		} else {
			supplementaryRel = rel - OFFSET;
		}

		prevSize = stmtTable[stmtB][supplementaryRel].size();
		stmtTable[stmtB][supplementaryRel].push_back(stmtA);
		success = (prevSize + 1 == stmtTable[stmtB][supplementaryRel].size()) && success;
	}

	return success;
}

bool PKB::putAssignForStmt(StmtNumber stmt, AssignTree tree) {
	while (stmt > assignTrees.size()) {
		assignTrees.push_back(AssignTree());
	}

	assignTrees.push_back(tree);

	return (stmt + 1 == assignTrees.size());
}
