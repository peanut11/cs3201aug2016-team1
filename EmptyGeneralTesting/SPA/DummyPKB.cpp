#include "DummyPKB.h"
#include "PKB.h"
#include <string>
#include <vector>
#include <set>

#include "AssignTree.h"
#include "EntityType.h"	
#include "RelationshipType.h"
#include "Types.h"


DummyPKB::DummyPKB()
{
}

VarIndex DummyPKB::getVarIndex(VarName varName) 
{
	if (varName == "x") {
		return 0;
	} else if (varName == "w") {
		return 1;
	} 
	return -1;
}

VarName DummyPKB::getVarName(VarIndex varIndex)
{
	if (varIndex == 0) {
		return "x";
	} else if (varIndex == 1) {
		return "w";
	}
	return "";
}

bool DummyPKB::is(RelationshipType rel, StmtNumber stmt, StmtVarIndex item)
{
	// case 1&7&9&10 : follows 
	if (rel == FOLLOWS && stmt == 3 && item == 4) {
		return true;
	}
	// case 7&9&10 : follows 
	else if (rel == FOLLOWS && stmt == 6 && item == 7) {
		return true;
	}
	// case 1&5 : modifies 
	else if (rel == MODIFIES && stmt == 3 && item == 0) {
		return true;
	}
	else if (rel == MODIFIES && stmt == 8 && item == 0) {
		return true;
	}
	else if (rel == MODIFIES && stmt == 4 && item == 1) {
		return true;
	}
	else if (rel == MODIFIES && stmt == 5 && item == 1) {
		return true;
	}
	return false;
}

bool DummyPKB::isAssignHasExpr(StmtNumber assign, ExprString expr)
{
	return false;
}

bool DummyPKB::isAssignHasSubexpr(StmtNumber assign, ExprString subexpr)
{
	if (assign == 8 && subexpr == "_z_") {
		return true;
	}
	else if (assign == 5 && subexpr == "_3_") {
		return true;
	}
	return false;
}

std::set<StmtNumber> DummyPKB::getAllStmts()
{
	std::set<StmtNumber> stmts = { 1,2,3,4,5,6,7,8,9 };
	return stmts;
}

std::set<VarName> DummyPKB::getAllVarNames()
{
	std::set<VarName> vars = { "x","y","w","z" };
	return vars;
}

std::set<StmtNumber> DummyPKB::getStmtsByType(EntityType stmtType)
{
	if (stmtType == ASSIGN) {
		std::set<StmtNumber> assigns = { 5,6,7,8,9 };
		return assigns;
	}
	else if (stmtType = STMT) {
		std::set<StmtNumber> stmts = { 1,2,3,4,5,6,7,8,9 };
		return stmts;
	}
	else if (stmtType = WHILE) {
		std::set<StmtNumber> whiles = { 2,3 };
		return whiles;
	}
}

std::set<StmtNumber> DummyPKB::getStmtsByVar(RelationshipType modifiesOrUses, VarName varName)
{
	if (modifiesOrUses == MODIFIES && varName == "x") {
		std::set<StmtNumber> cases = { 3,8 };
		return cases;
	}
	else if (modifiesOrUses == MODIFIES && varName == "w") {
		std::set<StmtNumber> cases = { 4,5 };
		return cases;
	}
	return std::set<StmtNumber>();
}

std::set<StmtNumber> DummyPKB::getStmtsByStmt(StmtNumber stmt, RelationshipType followsOrParent)
{
	// case 2&3
	if (followsOrParent == FOLLOWS && stmt == 3) {
		std::set<StmtNumber> cases = { 1,2,7 };
		return cases;
	}
	else if (followsOrParent == FOLLOWS && stmt == 10) {
		std::set<StmtNumber> cases = { 8,9 };
		return cases;
	}
	return std::set<StmtNumber>();
}

std::set<StmtNumber> DummyPKB::getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt)
{
	// case 4&5
	if (followsOrParent == FOLLOWS && stmt == 3) {
		std::set<StmtNumber> cases = { 4,5,8 };
		return cases;
	}
	return std::set<StmtNumber>();
}

std::set<VarIndex> DummyPKB::getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses)
{
	if (modifiesOrUses == MODIFIES && stmt == 3) {
		std::set<VarIndex> vars = { 0 };
		return vars;
	}
	return std::set<VarIndex>();
}
