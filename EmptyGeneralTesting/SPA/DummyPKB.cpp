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
	// "x" = 0
	// "w" = 1;
	if (varName == "x") {
		return 0;
	} else if (varName == "w") {
		return 1;
	} 
	return -1;
}

VarName DummyPKB::getVarName(VarIndex varIndex) {
	// 0 = "x"
	// 1 = "w"
    // 2 = "y"
    // 3 = "z"
	if (varIndex == 0) {
		return "x";
	} else if (varIndex == 1) {
		return "w";
    } else if (varIndex == 2) {
        return "y";
    } else if (varIndex == 3) {
        return "z";
    } else {
        return "";
    }
}

bool DummyPKB::is(RelationshipType rel, StmtNumber stmt, StmtVarIndex item)
{
	// Follows (3,4),(6,7)
	// Modifies (3,"x"), (8,"x"), (4,"w"), (5,"w")
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
	// pattern a(8,"_z_"), a(5,_3_)
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
	// statement : 1,2,3,4,5,6,7,8,9
	std::set<StmtNumber> stmts = { 1,2,3,4,5,6,7,8,9 };
	return stmts;
}

std::set<VarIndex> DummyPKB::getAllVarIndex() {
    return std::set<VarIndex>({ 0,1,2,3 });
}

std::set<VarName> DummyPKB::getAllVarNames()
{
	// variables : x,y,w,z
	std::set<VarName> vars = { "x","y","w","z" };
	return vars;
}

std::set<StmtNumber> DummyPKB::getStmtsByType(EntityType stmtType)
{
	// assignments = 5,6,7,8,9
	// statement : 1,2,3,4,5,6,7,8,9
	// while : 2,3
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
	// Modifies(s,"x") : s => 3,8
	// Modifies(s,"w") : s => 4,5
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
	// (3,follows) => 1,2,7 => Follows(s,3)
	// (10,follows) => 8,9 => Follows(s,3)
	// (7,follows) => 6 => Follows(s,7)
	if (followsOrParent == FOLLOWS && stmt == 3) {
		std::set<StmtNumber> cases = { 1,2,7 };
		return cases;
	}
	else if (followsOrParent == FOLLOWS && stmt == 10) {
		std::set<StmtNumber> cases = { 8,9 };
		return cases;
	}
	else if (followsOrParent == FOLLOWS && stmt == 7) {
		std::set<StmtNumber> cases = { 6 };
		return cases;
	}
	return std::set<StmtNumber>();
}

std::set<StmtNumber> DummyPKB::getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt)
{
	// (follows,3) => 4,5,8 => Follows(3,s)
	// case 4&5
	if (followsOrParent == FOLLOWS && stmt == 3) {
		std::set<StmtNumber> cases = { 4,5,8 };
		return cases;
	}
	return std::set<StmtNumber>();
}

std::set<VarIndex> DummyPKB::getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses)
{
	// (3,modifies) = "x" = Modifies(3,v)
	// (8,modifies) = "x" = Modifies(8,v)
	if (modifiesOrUses == MODIFIES && stmt == 3) {
		std::set<VarIndex> vars = { 0 };
		return vars;
	} else if (modifiesOrUses == MODIFIES && stmt == 8) {
		std::set<VarIndex> vars = { 0 };
		return vars;
	}
	return std::set<VarIndex>();
}
