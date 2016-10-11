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

bool PKB::is(RelationshipType rel, ProcStmtIndex stmtOrProcIndex, ProcStmtVarIndex item) {
    
	if (rel == FOLLOWS || rel == PARENT || rel == FOLLOWS_STAR || rel == PARENT_STAR) {
		if (stmtOrProcIndex >= stmtTable.size()) {
			return false;
		}

		rel = RelationshipType(rel + 1);
		StmtEntry entry = stmtTable[stmtOrProcIndex][rel];
		return entry.find(item) != entry.end();
	} else if (rel == CALLS || rel == CALLS_STAR) {
		if (stmtOrProcIndex >= procTable.size()) {
			return false;
		}
		return procTable[stmtOrProcIndex][rel].find(item) != procTable[stmtOrProcIndex][rel].end();
	} else {
		throw Exception::INVALID_RELATION;
	}

	
}

// Deprecated
bool PKB::isAssignHasExpr(StmtNumber assign, StringToken expr) {
	return false;
}

int operatorRank(StringToken s) {
	if (s == "+" || s == "-") {
		return 1;
	}

	if (s == "*" || s == "/") {
		return 2;
	}

	return 0; //not an operator
}

PostfixExpr PKB::infixToPostfix(InfixExpr infix) {
	PostfixExpr result;
	std::stack<StringToken> tokenStack;

	for (StringToken s : infix) {
		if (s == "(") {
			tokenStack.push(s);
		}
		else if (s == ")") {
			while (!tokenStack.empty() && tokenStack.top() != "(") {
				result.push_back(tokenStack.top());
				tokenStack.pop();
			}
			if (tokenStack.empty()) {
				// INVALID INFIX EXPR!!!!!!!!!!!!!!!!!
				return result;
			}
			tokenStack.pop();
		}
		else {
			int rank = operatorRank(s);
			if (rank == 0) { // is operand
				result.push_back(s);
			}
			else { // is operator
				while (!tokenStack.empty() && operatorRank(tokenStack.top()) >= operatorRank(s)) {
					result.push_back(tokenStack.top());
					tokenStack.pop();
				}
				tokenStack.push(s);
			}
		}
	}

	while (!tokenStack.empty()) {
		StringToken token = tokenStack.top();
		if (operatorRank(token) == 0) {
			// INVALID INFIX EXPR!!!!!!!!!
		}
		tokenStack.pop();
		result.push_back(token);
	}

	return result;
}

// Deprecated
bool PKB::isAssignHasSubexpr(StmtNumber assign, StringToken expr) {
	return false;
}

bool PKB::isAssignExactPattern(StmtNumber stmt, InfixExpr infixPattern) {
	PostfixExpr expr = postfixExprs[stmt],
		postfixPattern = infixToPostfix(infixPattern);

	if (postfixPattern.size() != expr.size()) {
		return false;
	}

	for (size_t i = 0; i < expr.size(); i++) {
		if (expr[i] != postfixPattern[i]) {
			return false;
		}
	}

	return true;
}

bool PKB::isAssignContainsPattern(StmtNumber stmt, InfixExpr infixPattern) {
	PostfixExpr expr = postfixExprs[stmt],
		postfixPattern = infixToPostfix(infixPattern);

	size_t patternSize = postfixPattern.size(),
		exprSize = expr.size();

	if (patternSize > exprSize) {
		return false;
	}

/*	for (size_t i = 0; i + patternSize <= exprSize; i++) {
		bool match = true;
		for (size_t j = 0; j < patternSize; j++) {
			if (postfixPattern[j] != expr[i+j]) {
				match = false;
				break;
			}
		}

		if (match) {
			return true;
		}
	} */

	// KMP algorithm
	std::vector<int> T(patternSize, 0);

	T[0] = -1;

	for (size_t i = 1; i < patternSize; i++) {
		int k = T[i - 1];
		while (k != -1 && postfixPattern[i] != postfixPattern[k+1]) {
			k = T[k];
		}
		T[i] = (postfixPattern[i] == postfixPattern[k + 1]) ? k + 1 : -1;
	}

	int match = -1;

	for (size_t i = 0; i < exprSize; i++) {
		while (match != -1 && expr[i] != postfixPattern[match+1]) {
			match = T[match];
		}
		if (expr[i] == postfixPattern[match + 1]) {
			match++;
		}
		if (match == patternSize - 1) {
			return true;
		}
	}

	return false;
}

bool PKB::isVarExist(VarName varName) {
	return (varRefMap.find(varName) != varRefMap.end());
}

bool PKB::isWhilePattern(StmtNumber whileStmt, VarIndex varIndex) {
	return getStmtTypeForStmt(whileStmt) == WHILE
        && varIndex < varRefTable.size()
        && controlVars[whileStmt] == varIndex;
}

bool PKB::isProcExist(ProcName procName) {
    return (procRefMap.find(procName) != procRefMap.end());
}

bool PKB::isStmtExist(StmtNumber stmt) {
    return (stmt < stmtTable.size());
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

std::set<ProcName> PKB::getAllProcNames() {
	return std::set<ProcName>(procRefTable.begin(), procRefTable.end());
}

EntityType PKB::getStmtTypeForStmt(StmtNumber stmt) {
    if (stmt >= stmtTypeTable.size()) {
        return INVALID;
    }

	return stmtTypeTable[stmt];
}

// Deprecated
StmtSet PKB::getStmtsByVar(RelationshipType rel, VarName varName) {
    return StmtSet();
	// return varTable[getVarIndex(varName)][rel];
}

StmtSet PKB::getStmtsByVar(RelationshipType rel, VarIndex varIndex) {
    if (rel != MODIFIES && rel != USES) {
        throw std::invalid_argument("");
    }

    if (varIndex >= varTable.size()) {
        throw std::out_of_range("");
    }

    return varTable[varIndex][rel];
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
        && followsOrParent != PARENT && followsOrParent!= PARENT_STAR
		&& followsOrParent != NEXT) {
		throw Exception::INCORRECT_PKB_API;
	}

    if (stmt >= stmtTable.size()) {
        return StmtSet();
    }

	int supplementaryRel = followsOrParent + 1;
	return stmtTable[stmt][supplementaryRel];
}

std::set<StmtNumber> PKB::getCallsByProc(ProcIndex procIndex) {
    return procToCallTable[procIndex];
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
StmtNumber PKB::getProcTableSize() {
	return procTable.size() - 1;
}

ProcIndex PKB::getProcByCall(StmtNumber callStmt) {
    return callToProcMap[callStmt];
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
        procToCallTable.push_back(StmtSet()); 
        procToStmtTable.push_back(StmtSet());
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

std::set<VarIndex> PKB::getVarsByProc(ProcIndex procIndex, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_PROC_RELATION;
	}

	return procTable[procIndex][modifiesOrUses];
}

std::set<ProcIndex>	PKB::getProcsByProc(ProcName procName, RelationshipType calls) {
	if (calls != CALLS && calls != CALLS_STAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	return procTable[getProcIndex(procName)][calls];
}

std::set<ProcIndex>	PKB::getProcsByProc(ProcIndex procIndex, RelationshipType calls) {
	if (calls != CALLS && calls != CALLS_STAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	return procTable[procIndex][calls];
}

std::set<ProcIndex>	PKB::getProcsByProc(RelationshipType calls, ProcName procName ) {
	if (calls != CALLS && calls != CALLS_STAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	return procTable[getProcIndex(procName)][calls + 1];
}

std::set<ProcIndex>	PKB::getProcsByProc(RelationshipType calls, ProcIndex procIndex) {
	if (calls != CALLS && calls != CALLS_STAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	return procTable[procIndex][calls + 1];
}

std::set<ProcIndex> PKB::getProcsByVar(RelationshipType modifiesOrUses, VarName varName) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_PROC_RELATION;
	}

	return varTable[getVarIndex(varName)][modifiesOrUses + 2];
}

std::set<ProcIndex> PKB::getProcsByVar(RelationshipType modifiesOrUses, VarIndex varIndex) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw Exception::INVALID_VAR_PROC_RELATION;
	}

	return varTable[varIndex][modifiesOrUses + 2];
}

// Deprecated
std::set<StmtNumber> PKB::getStmtsByProc(ProcName procName) {
    return StmtSet();
    // return procToStmtTable[getProcIndex(procName)];
}

std::set<StmtNumber> PKB::getStmtsByProc(ProcIndex procIndex) {
    return procToStmtTable[procIndex];
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
	stmtTable[stmt][rel].insert(varIndex);
	bool success = (stmtTable[stmt][rel].find(varIndex) != stmtTable[stmt][rel].end());

	varTable[varIndex][rel].insert(stmt);
    success = (varTable[varIndex][rel].find(stmt) != varTable[varIndex][rel].end()) && success;

	varTable[varIndex][rel + 2].insert(procIndex); // Adds Modified/UsedByProc
	success = (varTable[varIndex][rel + 2].find(procIndex) != varTable[varIndex][rel + 2].end()) && success;

	procTable[procIndex][rel].insert(varIndex);
	success = (procTable[procIndex][rel].find(varIndex) != procTable[procIndex][rel].end()) && success;

	return success;
}

bool PKB::putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB) {
	if (rel == MODIFIES || rel == USES || rel == CALLS) {
		throw Exception::INVALID_STMT_STMT_RELATION;
	}

	if (rel == FOLLOWED_BY || rel == PARENT_OF || rel == PREVIOUS) {
		throw std::runtime_error(""); // INTERNAL_USE_ERROR
	}

	while (stmtB >= stmtTable.size() || stmtA >= stmtTable.size()) {
		stmtTable.push_back(StmtRow());
	}

	bool success = stmtTable[stmtA][rel].insert(stmtB).second;

	if (rel == FOLLOWS || rel == FOLLOWS_STAR || rel == PARENT || rel == PARENT_STAR || rel == NEXT) {
		const int OFFSET = 1;
		int supplementaryRel;

		if (rel == FOLLOWS || rel == PARENT || rel == NEXT) {
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

bool PKB::putStmtCallProc(StmtNumber stmt, ProcName procCalled) {
	bool success = true;
	ProcIndex procIndex = getProcIndex(procCalled);
	callToProcMap[stmt] = procIndex;
	success = (callToProcMap.find(stmt) != callToProcMap.end()) && success;
	procToCallTable[procIndex].insert(stmt);
	success = (procToCallTable[procIndex].find(stmt) != procToCallTable[procIndex].end()) && success;
	return success;
}

/*bool PKB::putAssignForStmt(StmtNumber stmt, AssignTree tree) {
	while (stmt > assignTrees.size()) {
		assignTrees.push_back(AssignTree());
	}

	assignTrees.push_back(tree);
	return (stmt + 1 == assignTrees.size());
} */

bool PKB::putExprForStmt(StmtNumber stmt, PostfixExpr expr) {
	while (stmt > postfixExprs.size()) {
		postfixExprs.push_back(PostfixExpr());
	}

	postfixExprs.push_back(expr);
	return true;
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

bool PKB::putProcForProc(ProcIndex procA, RelationshipType callsOrStar, ProcName procB) {
	if (callsOrStar != CALLS && callsOrStar != CALLS_STAR) {
		throw Exception::INVALID_PROC_PROC_RELATION;
	}

	ProcIndex procIndexA = procA;
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
