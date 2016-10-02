// Maintained by: Kai Lin
//
// Facade of:
// - AssignmentTrees
// - StmtTable
// - VarTable

#include "PKB.h"
#include "algorithm"

const std::runtime_error PKB::ERROR = std::runtime_error("");

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
	varRefMap = VarRefMap();
	varRefTable = std::vector<VarName>();
	stmtTable = std::vector<StmtRow>();
	stmtTypeTable = std::vector<EntityType>();
	varTable = std::vector<VarRow>();
	procRefMap = ProcRefMap();
	procRefTable = std::vector<ProcName>();
	procTable = std::vector<ProcRow>();
	stmtToProcTable = std::vector<ProcIndex>();
	procToStmtTable = std::vector<std::set<StmtNumber>>();

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
	if (rel == FOLLOWS || rel == PARENT || rel == FOLLOWS_STAR || rel == PARENT_STAR) {
		rel = RelationshipType ((int)rel + 1);
	}

	StmtEntry::iterator it;
	StmtEntry entry = stmtTable[stmt][rel];
	it = entry.find(item);
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
	return std::set<VarName>(varRefTable.begin(),varRefTable.end());
}

AssignTree PKB::getAssign(StmtNumber stmt) {
	if (stmtTypeTable[stmt] != EntityType::ASSIGN) {
		throw ERROR;
	}

	return assignTrees[stmt];
}

EntityType PKB::getStmtTypeForStmt(StmtNumber stmt) {
	return stmtTypeTable[stmt];
}

std::set<StmtNumber> PKB::getStmtsByVar(RelationshipType rel, VarName varName) {
	return varTable[getVarIndex(varName)][rel];
}

std::set<StmtNumber> PKB::getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel) {
	if (stmtRel == MODIFIES || stmtRel == USES) {
		throw ERROR;
	}

	return stmtTable[stmt][stmtRel];
}

std::set<StmtNumber> PKB::getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt) {
	if (followsOrParent != FOLLOWS && followsOrParent != PARENT && followsOrParent != FOLLOWS_STAR && followsOrParent!= PARENT_STAR) {
		throw ERROR;
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
	}
	else {
		procIndex = it->second;
	}

	return procIndex;
}

VarName PKB::getVarName(VarIndex varIndex) {
	if (varIndex >= varRefTable.size()) {
		throw ERROR;
	}

	VarName varName = varRefTable[varIndex];
	return varName;
}

ProcName PKB::getProcName(ProcIndex procIndex) {
	if (procIndex >= procRefTable.size()) {
		throw ERROR;
	}

	ProcName procName = procRefTable[procIndex];
	return procName;
}

std::set<VarIndex> PKB::getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw ERROR;
	}

	return stmtTable[stmt][modifiesOrUses];
}

std::set<VarIndex> PKB::getVarsByProc(ProcName procName, RelationshipType modifiesOrUses) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw ERROR;
	}
	return procTable[getProcIndex(procName)][modifiesOrUses];
}

std::set<ProcIndex>	PKB::getProcsByProc(ProcName procName, RelationshipType calls) {
	if (calls != CALLS && calls != CALLSSTAR) {
		throw ERROR;
	}
	return procTable[getProcIndex(procName)][calls];
}

std::set<ProcIndex> PKB::getProcsByVar(RelationshipType modifiesOrUses, VarName varName) {
	if (modifiesOrUses != MODIFIES && modifiesOrUses != USES) {
		throw ERROR;
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
	return stmtToProcTable[stmt];
}

bool PKB::putVarForStmt(StmtNumber stmt, RelationshipType rel, VarName varName) {
	bool success; 
	int prevSize;
	VarIndex varIndex = getVarIndex(varName);

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
	stmtTable[stmt][rel].insert(varIndex);
	success = (prevSize + 1 == stmtTable[stmt][rel].size());

	prevSize = varTable[varIndex][rel].size();
	varTable[varIndex][rel].insert(stmt);
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
	stmtTable[stmtA][rel].insert(stmtB);
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
		stmtTable[stmtB][supplementaryRel].insert(stmtA);
		success = (prevSize + 1 == stmtTable[stmtB][supplementaryRel].size()) && success;
	}

	return success;
}

bool PKB::putStmtTypeForStmt(StmtNumber stmt, EntityType stmtType) {
	if (stmtTypeTable.size() != stmt) {
		throw ERROR;
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