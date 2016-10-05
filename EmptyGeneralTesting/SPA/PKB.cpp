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
    controlVars = std::vector<VarIndex>();
    procRefMap = ProcRefMap();
    procRefTable = std::vector<ProcName>();
    procToStmtTable = std::vector<std::set<StmtNumber>>(); 
    procTable = std::vector<ProcRow>();
    stmtByTypeTable = std::vector<StmtSet>(); 
    stmtTable = std::vector<StmtRow>();
    stmtToProcTable = std::vector<ProcIndex>(); 
    stmtTypeTable = std::vector<EntityType>();
    varRefMap = VarRefMap();
	varRefTable = std::vector<VarName>();
	varTable = std::vector<VarRow>();

	stmtTable.push_back(StmtRow());      // StmtNumber starts from 1
	stmtTypeTable.push_back(INVALID);    // StmtNumber starts from 1
	stmtToProcTable.push_back(UINT_MAX); // StmtNumber starts from 1. Not sure of better invalid values to put here

    while (stmtByTypeTable.size() <= EntityType::STMT) {
        stmtByTypeTable.push_back(StmtSet());
    }
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
		rel = RelationshipType(rel + 1);
	}

	StmtEntry entry = stmtTable[stmt][rel];
	return entry.find(item) != entry.end();
}

bool PKB::isAssignHasExpr(StmtNumber assign, ExprString expr) {
    if (expr[0] == '_') {
        return isAssignHasSubexpr(assign, expr.substr(1, expr.size() - 2));
    }

    AssignTree tree = assignTrees[assign];
	ExprString treeStr = ExprTree::toString(tree.getExprTree());

    expr.erase(std::remove_if(expr.begin(), expr.end(), isspace), expr.end());
	return expr == treeStr;
}

bool PKB::isAssignHasSubexpr(StmtNumber assign, ExprString subexpr) {
    if (subexpr[0] == '_') {
        return isAssignHasSubexpr(assign, subexpr.substr(1, subexpr.size() - 2));
    }

	AssignTree tree = assignTrees[assign];
	ExprString treeStr = ExprTree::toString(tree.getExprTree());
	treeStr = "+" + treeStr + "+";

    subexpr.erase(std::remove_if(subexpr.begin(), subexpr.end(), isspace), subexpr.end());

	return (treeStr.find(subexpr) == 1) ||
		(subexpr.find("+") == std::string::npos && treeStr.find("+" + subexpr + "+") != std::string::npos);
}

bool PKB::isVarExist(VarName varName) {
	return (varRefMap.find(varName) != varRefMap.end());
}

bool PKB::isWhilePattern(StmtNumber whileStmt, VarIndex varIndex) {
	return getStmtTypeForStmt(whileStmt) == WHILE
        && varIndex < varRefTable.size()
        && controlVars[whileStmt] == varIndex;
}

bool PKB::isIfPattern(StmtNumber ifStmt, VarIndex varIndex) {
	return getStmtTypeForStmt(ifStmt) == IF
        && varIndex < varRefTable.size()
        && controlVars[ifStmt] == varIndex;
}

std::set<Constant> PKB::getAllConstantValues() {
	return constants;
}

StmtSet PKB::getAllStmts() {
	return stmtByTypeTable[STMT];
}

std::set<VarIndex> PKB::getAllVarIndex() {
    std::set<VarIndex> allVarIndex;
    for (VarIndex varIndex = 0; varIndex < varRefTable.size(); varIndex++) {
        allVarIndex.insert(varIndex);
    }

    return allVarIndex;
}

std::set<VarName> PKB::getAllVarNames() {
	return std::set<VarName>(varRefTable.begin(),varRefTable.end());
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

StmtSet PKB::getStmtsByVar(RelationshipType rel, VarName varName) {
	return varTable[getVarIndex(varName)][rel];
}

StmtSet PKB::getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel) {
	if (stmtRel == MODIFIES || stmtRel == USES) {
		throw Exception::INVALID_STMT_STMT_RELATION;
	}

    if (stmt >= stmtTable.size()) {
        return StmtSet();
    }

	return stmtTable[stmt][stmtRel];
}

StmtSet PKB::getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt) {
	if (followsOrParent != FOLLOWS && followsOrParent != FOLLOWS_STAR
        && followsOrParent != PARENT && followsOrParent!= PARENT_STAR) {
		throw Exception::INCORRECT_PKB_API;
	}

    if (stmt >= stmtTable.size()) {
        return StmtSet();
    }

	int supplementaryRel = followsOrParent + 1;
	return stmtTable[stmt][supplementaryRel];
}

StmtSet PKB::getStmtsByType(EntityType stmtType) {
    if (stmtType > STMT) {
        throw Exception::INVALID_STMT_TYPE;
    }

	return stmtByTypeTable[stmtType];
}

StmtNumber PKB::getStmtTableSize() {
	return stmtTable.size() - 1; // StmtNumber starts from 1
}

VarIndex PKB::getVarIndex(VarName varName) {
	VarRefMap::const_iterator it = varRefMap.find(varName);
	VarIndex varIndex;
	
	if (it == varRefMap.end()) {
		varIndex = varTable.size();
		varTable.push_back(VarRow());
		varRefTable.push_back(varName);
		varRefMap[varName] = varIndex;
	} else {
		varIndex = it->second;
	}

	return varIndex;
}

ProcIndex PKB::getProcIndex(ProcName procName) {
	ProcRefMap::const_iterator it = procRefMap.find(procName);
	ProcIndex procIndex;

	if (it == procRefMap.end()) {
		procIndex = procTable.size();
		procTable.push_back(ProcRow());
		procRefTable.push_back(procName);
		procRefMap[procName] = procIndex;
        procToStmtTable.push_back(std::set<StmtNumber>());
	} else {
		procIndex = it->second;
	}

	return procIndex;
}

VarName PKB::getVarName(VarIndex varIndex) {
	if (varIndex >= varRefTable.size()) {
		throw Exception::INVALID_VAR_INDEX;
	}

	return varRefTable[varIndex];
}

ProcName PKB::getProcName(ProcIndex procIndex) {
	if (procIndex >= procRefTable.size()) {
		throw Exception::INVALID_PROC_INDEX;
	}

	return procRefTable[procIndex];
}

std::set<VarIndex> PKB::getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_STMT_RELATION;
	}

    if (stmt >= stmtTable.size()) {
        return std::set<VarIndex>();
    }

	return stmtTable[stmt][modifiesOrUses];
}

std::set<VarIndex> PKB::getVarsByProc(ProcName procName, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_PROC_RELATION;
	}

	return procTable[getProcIndex(procName)][modifiesOrUses];
}

std::set<ProcIndex>	PKB::getProcsByProc(ProcName procName, RelationshipType calls) {
	if (calls != CALLS && calls != CALLS_STAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	return procTable[getProcIndex(procName)][calls];
}

std::set<ProcIndex> PKB::getProcsByVar(RelationshipType modifiesOrUses, VarName varName) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_PROC_RELATION;
	}

    std::set<ProcIndex> procedures;

	// Get the stmts that modifiesOrUses varName
	std::set<StmtNumber> stmts = getStmtsByVar(modifiesOrUses, varName);

	// Iterate through the stmts to get procedures
	for each (StmtNumber stmt in stmts) {
		procedures.insert(stmtToProcTable[stmt]);
	}

	return procedures;
}

std::set<StmtNumber> PKB::getStmtsByProc(ProcName procName) {
	return procToStmtTable[getProcIndex(procName)];
}

ProcIndex PKB::getProcByStmt(StmtNumber stmt) {
	if (stmt >= stmtToProcTable.size()) {
		throw Exception::UNEXPECTED_STMT_ERROR;
	}

	return stmtToProcTable[stmt];
}

bool PKB::putVarForStmt(StmtNumber stmt, RelationshipType rel, VarName varName) {
	VarIndex varIndex = getVarIndex(varName);
	ProcIndex procIndex = getProcByStmt(stmt);

	if (rel != MODIFIES && rel != USES) {
		throw Exception::INVALID_VAR_STMT_RELATION;
	}
	
	while (stmt >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}
	
	// Update stmtTable, varTable, procTable
	stmtTable[stmt][rel].emplace(varIndex);
	bool success = stmtTable[stmt][rel].find(varIndex) != stmtTable[stmt][rel].end();

	varTable[varIndex][rel].emplace(stmt);
    success = (varTable[varIndex][rel].find(stmt) != varTable[varIndex][rel].end()) && success;
	procTable[procIndex][rel].emplace(varIndex);
	success = (procTable[procIndex][rel].find(varIndex) != procTable[procIndex][rel].end()) && success;

	return success;
}

bool PKB::putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB) {
	if (rel == MODIFIES || rel == USES) {
		throw Exception::INVALID_STMT_STMT_RELATION;
	}

	while (stmtB >= stmtTable.size() || stmtA >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}

	bool success = stmtTable[stmtA][rel].insert(stmtB).second;

	if (rel == FOLLOWS || rel == PARENT || rel == FOLLOWED_BY || rel == PARENT_OF) {
		const int OFFSET = 1;
		int supplementaryRel;

		if (rel == FOLLOWS || rel == PARENT) {
			supplementaryRel = rel + OFFSET;
		} else {
			supplementaryRel = rel - OFFSET;
		}

		success = stmtTable[stmtB][supplementaryRel].insert(stmtA).second && success;
	}

	return success;
}

bool PKB::putStmtTypeForStmt(StmtNumber stmt, EntityType stmtType) {
	if (stmtTypeTable.size() != stmt) {
		throw Exception::UNEXPECTED_STMT_ERROR;
	}

    bool success = stmtByTypeTable[stmtType].insert(stmt).second;
    success = stmtByTypeTable[STMT].insert(stmt).second && success;

    stmtTypeTable.push_back(stmtType);
    success = (stmt + 1 == stmtTypeTable.size()) && success;

	return success;
}

bool PKB::putAssignForStmt(StmtNumber stmt, AssignTree tree) {
	while (stmt > assignTrees.size()) {
		assignTrees.push_back(AssignTree());
	}

	assignTrees.push_back(tree);
	return (stmt + 1 == assignTrees.size());
}

bool PKB::putConstant(Constant constant) {
    return constants.insert(constant).second;
}

bool PKB::putStmtProc(StmtNumber stmt, ProcName procNameContainingStmt) {
	ProcIndex procIndex = getProcIndex(procNameContainingStmt);
	
    // Update ProcToStmtTable
    bool success = procToStmtTable[procIndex].insert(stmt).second;

	// Update StmtToProcTable
	stmtToProcTable.push_back(procIndex);
	success = (stmt + 1 == stmtToProcTable.size()) && success;

	return success;
}

bool PKB::putProcForProc(ProcName procA, RelationshipType callsOrStar, ProcName procB) {
	if (callsOrStar != CALLS && callsOrStar != CALLS_STAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	ProcIndex procIndexA = getProcIndex(procA);
	ProcIndex procIndexB = getProcIndex(procB);

	// Updates A CALLS B
	bool success = procTable[procIndexA][callsOrStar].insert(procIndexB).second;

	// Updates B CALLED_BY A
	success = procTable[procIndexB][callsOrStar + 1].insert(procIndexA).second && success;

	return success;
}

bool PKB::putControlVarForStmt(StmtNumber ifOrWhile, VarName varName) {
	EntityType stmtType = getStmtTypeForStmt(ifOrWhile);

	if (stmtType != IF && stmtType != WHILE) {
		throw Exception::NOT_WHILE_OR_IF_ERROR;
	}

	while (controlVars.size() < ifOrWhile) {
		controlVars.push_back(UINT_MAX);
	}

	controlVars.push_back(getVarIndex(varName));
	return (ifOrWhile + 1 == controlVars.size());
}
