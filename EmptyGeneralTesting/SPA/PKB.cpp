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
	controlVars = std::vector<VarIndex>();
	constants = std::set<Constant>();
	varRefMap = VarRefMap();
	varRefTable = std::vector<VarName>();
	stmtTable = std::vector<StmtRow>();
	stmtTypeTable = std::vector<EntityType>();
	stmtByTypeTable = std::vector<StmtSet>();
	varTable = std::vector<VarRow>();
	procRefMap = ProcRefMap();
	procRefTable = std::vector<ProcName>();
	procTable = std::vector<ProcRow>();
	stmtToProcTable = std::vector<ProcIndex>();
	procToStmtTable = std::vector<std::set<StmtNumber>>();

	stmtTable.push_back(StmtRow());   // StmtNumber starts from 1
	stmtTypeTable.push_back(INVALID); // StmtNumber starts from 1
	stmtToProcTable.push_back(UINT_MAX); //StmtNumber starts from 1. Not sure of better invalid values to put here

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
	return (varRefMap.find(varName) != varRefMap.end());
}

bool PKB::isWhilePattern(StmtNumber whileStmt, VarIndex varIndex) {
	return getStmtTypeForStmt(whileStmt) == WHILE && varIndex <varRefTable.size() && controlVars[whileStmt] == varIndex;
}
bool PKB::isIfPattern(StmtNumber ifStmt, VarIndex varIndex) {
	return getStmtTypeForStmt(ifStmt) == IF && varIndex < varRefTable.size() && controlVars[ifStmt] == varIndex;
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
        allVarIndex.emplace(varIndex);
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
	}
	else {
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

	ProcName procName = procRefTable[procIndex];
	return procName;
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
	if (calls != CALLS && calls != CALLSSTAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}
	return procTable[getProcIndex(procName)][calls];
}

std::set<ProcIndex> PKB::getProcsByVar(RelationshipType modifiesOrUses, VarName varName) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_PROC_RELATION;
	}
	std::set<ProcIndex> procedures;

	// gets the stmts that modifiesOrUses varName
	std::set<StmtNumber> stmts = getStmtsByVar(modifiesOrUses, varName);

	// iterates through the stmts to get procedures
	for each(StmtNumber stmt in stmts) {
		procedures.insert(stmtToProcTable[stmt]);
	}
	return procedures;
}

std::set<StmtNumber> PKB::getStmtsByProc(ProcName procName) {
	ProcIndex procIndex = getProcIndex(procName);
	return procToStmtTable[procIndex];
}

ProcIndex PKB::getProcByStmt(StmtNumber stmt) {
	if (stmt >= stmtToProcTable.size()) {
		throw Exception::UNEXPECTED_STMT_ERROR;
	}
	return stmtToProcTable[stmt];
}

bool PKB::putVarForStmt(StmtNumber stmt, RelationshipType rel, VarName varName) {
	bool success; 
	int prevSize;
	VarIndex varIndex = getVarIndex(varName);
	ProcIndex procIndex = getProcByStmt(stmt);

	if (rel != MODIFIES && rel != USES) {
		throw Exception::INVALID_VAR_STMT_RELATION;
	}
	
	while (stmt >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}

	while (varIndex >= varTable.size()) { // unnecessary but just in case lol
		varTable.push_back(VarRow());
	}

	while (procIndex >= procTable.size()) { //unnecessary but just in case lol
		procTable.push_back(ProcRow());
	}
	
	// update StmtTable
	prevSize = stmtTable[stmt][rel].size();
	stmtTable[stmt][rel].insert(varIndex);
	success = stmtTable[stmt][rel].find(varIndex) != stmtTable[stmt][rel].end();

	// update VarTable
	prevSize = varTable[varIndex][rel].size();
	varTable[varIndex][rel].insert(stmt);
	success = (varTable[varIndex][rel].find(stmt) != varTable[varIndex][rel].end()) && success;

	// update ProcTable
	prevSize = procTable[procIndex][rel].size();
	procTable[procIndex][rel].insert(varIndex);
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

    int prevSize = stmtByTypeTable[stmtType].size();
    stmtByTypeTable[stmtType].emplace_hint(stmtByTypeTable[stmtType].end(), stmt);
    bool success = (prevSize + 1 == stmtByTypeTable[stmtType].size());

    stmtByTypeTable[STMT].emplace_hint(stmtByTypeTable[STMT].end(), stmt);
    success = (stmt + 1 == stmtByTypeTable[STMT].size()) && success;

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
	int prevSize = constants.size();
	constants.insert(constant);
	return constants.find(constant) != constants.end();
}

bool PKB::putStmtProc(StmtNumber stmt, ProcName procNameContainingStmt) {
	ProcIndex procIndex = getProcIndex(procNameContainingStmt);
	bool success;
	int prevSize;
	
	// assumes previous entries were all inserted correctly
	
	// update StmtToProcTable
	prevSize = stmtToProcTable.size();
	stmtToProcTable.push_back(procIndex);
	success = (prevSize + 1 == stmtToProcTable.size());

	// update ProcToStmtTable
	prevSize = procToStmtTable[procIndex].size();
    procToStmtTable[procIndex].emplace_hint(procToStmtTable[procIndex].end(), stmt);
	success = (prevSize + 1 == procToStmtTable[procIndex].size() && success); // size should always increase as there should be no repetition of stmt
	return success;
}

bool PKB::putProcForProc(ProcName procA, RelationshipType calls, ProcName procB) {
	if (calls != CALLS && calls != CALLSSTAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	bool success;
	int prevSize;
	ProcIndex procIndexA = getProcIndex(procA);
	ProcIndex procIndexB = getProcIndex(procB);
	ProcIndex it;

	// updates A CALLS B
	procTable[procIndexA][calls].insert(procIndexB);
	success = procTable[procIndexA][calls].find(procIndexB) != procTable[procIndexA][calls].end();

	// updates B CALLED_BY A
	procTable[procIndexB][calls + 1].insert(procIndexA);
	success = procTable[procIndexB][calls + 1].find(procIndexA) != procTable[procIndexB][calls + 1].end();

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

	int prevSize = controlVars.size();
	controlVars.push_back(getVarIndex(varName));
	return prevSize + 1 == controlVars.size();
}