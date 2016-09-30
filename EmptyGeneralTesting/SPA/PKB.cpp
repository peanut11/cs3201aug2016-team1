// Maintained by: Kai Lin
//
// Facade of:
// - AssignmentTrees
// - StmtTable
// - VarTable

#include "PKB.h"

PKB* PKB::theOne = nullptr;

PKB* PKB::getInstance() {
	if (theOne == nullptr) {
		theOne = new PKB();
	}

	return theOne;
}

PKB::PKB() {
	assignTrees = std::vector<AssignTree>();
	constants = std::set<Constant>();
	refMap = RefMap();
	refTable = std::vector<VarName>();
	stmtTable = std::vector<StmtRow>();
	stmtTypeTable = std::vector<EntityType>();
	varTable = std::vector<VarRow>();

	stmtTable.push_back(StmtRow());   // StmtNumber starts from 1
	stmtTypeTable.push_back(INVALID); // StmtNumber starts from 1
}

void PKB::clear() {
	if (theOne != nullptr) {
		delete theOne;
	}
	theOne = new PKB();
}

bool PKB::is(RelationshipType rel, StmtNumber stmt, StmtVarIndex item) {
    if (stmt >= stmtTable.size()) {
        return false;
    }

	if (rel == FOLLOWS || rel == PARENT || rel == FOLLOWS_STAR || rel == PARENT_STAR) {
		rel = RelationshipType ((int)rel + 1);
	}

	StmtEntry entry = stmtTable[stmt][rel];
    StmtEntry::iterator it = entry.find(item);
	return it != entry.end();
}

bool PKB::isAssignHasExpr(StmtNumber assign, ExprString expr) {
	if (expr[0] == '_') return isAssignHasSubexpr(assign, expr.substr(1, expr.size() - 2));
	AssignTree tree = assignTrees[assign];
	expr.erase(std::remove_if(expr.begin(), expr.end(), isspace), expr.end());
	ExprString treeStr = ExprTree::toString(tree.getExprTree());

	return expr==treeStr;
}

bool PKB::isAssignHasSubexpr(StmtNumber assign, ExprString subexpr) {
	if (subexpr[0] == '_') return isAssignHasSubexpr(assign, subexpr.substr(1, subexpr.size() - 2));

	AssignTree tree = assignTrees[assign];
	subexpr.erase(std::remove_if(subexpr.begin(), subexpr.end(), isspace), subexpr.end());
	ExprString treeStr = ExprTree::toString(tree.getExprTree());

	treeStr = "+" + treeStr + "+";
	return (treeStr.find(subexpr) == 1) ||
		(subexpr.find("+") == std::string::npos && 
			treeStr.find("+" + subexpr + "+") != std::string::npos);
}

bool PKB::isVarExist(VarName varName) {
	return (refMap.find(varName) != refMap.end());
}

std::set<Constant> PKB::getAllConstantValues() {
	return constants;
}

std::set<StmtNumber> PKB::getAllStmts() {
	std::set<StmtNumber> stmts;
	for (StmtNumber stmt = 1; stmt < stmtTable.size(); stmt++) {
		stmts.insert(stmt);
	}
	return stmts;
}

std::set<VarName> PKB::getAllVarNames() {
	return std::set<VarName>(refTable.begin(),refTable.end());
}

AssignTree PKB::getAssign(StmtNumber stmt) {
	if (stmtTypeTable[stmt] != EntityType::ASSIGN) {
		throw Exception::NOT_ASSIGN_ERROR;
	}

	return assignTrees[stmt];
}

EntityType PKB::getStmtTypeForStmt(StmtNumber stmt) {
    if (stmt >= stmtTypeTable.size()) {
        return INVALID;
    }

	return stmtTypeTable[stmt];
}

std::set<StmtNumber> PKB::getStmtsByVar(RelationshipType rel, VarName varName) {
	return varTable[getVarIndex(varName)][rel];
}

std::set<StmtNumber> PKB::getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel) {
	if (stmtRel == MODIFIES || stmtRel == USES) {
		throw Exception::INVALID_STMT_RELATION;
	}

    if (stmt >= stmtTable.size()) {
        return std::set<StmtNumber>();
    }

	return stmtTable[stmt][stmtRel];
}

std::set<StmtNumber> PKB::getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt) {
	if (followsOrParent != FOLLOWS && followsOrParent != FOLLOWS_STAR
        && followsOrParent != PARENT && followsOrParent!= PARENT_STAR) {
		throw Exception::INCORRECT_PKB_API;
	}

    if (stmt >= stmtTable.size()) {
        return std::set<StmtNumber>();
    }

	int supplementaryRel = followsOrParent + 1;
	return stmtTable[stmt][supplementaryRel];
}

std::set<StmtNumber> PKB::getStmtsByType(EntityType stmtType) {
	std::set<StmtNumber> stmts;
	
	if (stmtType == STMT) {
		stmts = getAllStmts();

	} else {
		for (StmtNumber i = 1; i < stmtTable.size(); i++) { // StmtNumber starts from 1
			if (stmtTypeTable[i] == stmtType) {
				stmts.insert(i);
			}
		}
	}

	return stmts;
}

StmtNumber PKB::getStmtTableSize() {
	return stmtTable.size() - 1; // StmtNumber starts from 1
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
		throw Exception::INVALID_VAR_INDEX;
	}

	VarName varName = refTable[varIndex];
	return varName;
}

std::set<VarIndex> PKB::getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_RELATION;
	}

    if (stmt >= stmtTable.size()) {
        return std::set<VarIndex>();
    }

	return stmtTable[stmt][modifiesOrUses];
}

bool PKB::putVarForStmt(StmtNumber stmt, RelationshipType rel, VarName varName) {
	bool success; 
	int prevSize;
	VarIndex varIndex = getVarIndex(varName);

	if (rel != MODIFIES && rel != USES) {
		throw Exception::INVALID_VAR_RELATION;
	}
	
	while (stmt >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}

	while (varIndex >= varTable.size()) {
		varTable.push_back(VarRow());
	}
	
	prevSize = stmtTable[stmt][rel].size();
	stmtTable[stmt][rel].insert(varIndex);
	success = (prevSize + 1 == stmtTable[stmt][rel].size());

	prevSize = varTable[varIndex][rel].size();
	varTable[varIndex][rel].insert(stmt);
	success = (prevSize + 1 == varTable[varIndex][rel].size()) && success;

	return success;
}

bool PKB::putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB) {
	if (rel == MODIFIES || rel == USES) {
		throw Exception::INVALID_STMT_RELATION;
	}

	while (stmtB >= stmtTable.size() || stmtA >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}

	int prevSize = stmtTable[stmtA][rel].size();
	stmtTable[stmtA][rel].insert(stmtB);
	bool success = (prevSize + 1 == stmtTable[stmtA][rel].size());

	if (rel == FOLLOWS || rel == PARENT || rel == FOLLOWED_BY || rel == PARENT_OF) {
		const int OFFSET = 1;
		int supplementaryRel;

		if (rel == FOLLOWS || rel == PARENT) {
			supplementaryRel = rel + OFFSET;
		} else {
			supplementaryRel = rel - OFFSET;
		}

		prevSize = stmtTable[stmtB][supplementaryRel].size();
		stmtTable[stmtB][supplementaryRel].insert(stmtA);
		success = (prevSize + 1 == stmtTable[stmtB][supplementaryRel].size()) && success;
	}

	return success;
}

bool PKB::putStmtTypeForStmt(StmtNumber stmt, EntityType stmtType) {
	if (stmtTypeTable.size() != stmt) {
		throw Exception::UNEXPECTED_STMT_ERROR;
	}

	stmtTypeTable.push_back(stmtType);
	return (stmtTypeTable.size() == stmt+1);
}

bool PKB::putAssignForStmt(StmtNumber stmt, AssignTree tree) {
	while (stmt > assignTrees.size()) {
		assignTrees.push_back(AssignTree());
	}

	assignTrees.push_back(tree);

	return (stmt + 1 == assignTrees.size());
}

bool PKB::putConstant(Constant constant) {
	int prevSize = constants.size();
	constants.insert(constant);
	return (prevSize + 1 == constants.size());
}
