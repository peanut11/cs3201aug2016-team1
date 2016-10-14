#include "DummyPKB.h"
#include "PKB.h"
#include <string>
#include <vector>
#include <set>
#include "EntityType.h"	
#include "RelationshipType.h"
#include "Types.h"
/* 
		FILE : procedure2_iteration.txt

		procedure First {
		1.	  x = 2;
		2.	  z = 3;
		3.	  call Second; }

		procedure Second {
		4.	  x = 0;
		5.	  i = 5;
		6.	  while i {
		7.		x = x + 2 * y;
		8.		call Third;
		9.		i = i - 1; }
		10.	  if x then {
		11.		x = x + 1; }
		else {
		12.		z = 1; }
		13.	  z = z + x + i;
		14.	  y = z + 2;
		15.	  x = x * y + z; }

		procedure Third {
		16.	  z = 5;
		17.	  v = z; }

*/

/*		BASED ON : procedure_iteration2.txt
		------------------------
		variables // varIndex
		i			0
		v			1
		x			2
		y			3
		z			4
		------------------------
		procedures // procIndex
		First		0
		Second		1
		Third		2
		------------------------
		statements
		1-17
		------------------------
		assign-statements
		1,2,4,5,7,9,11,12,13,14,15,16,17
		------------------------
		if-statements
		10
		------------------------
		call-statements
		3,8
		------------------------
		while-statements
		6
		------------------------
		constants
		0,1,2,3,5
		------------------------
		Follows (1,2)
		Follows (2,3)
		-
		Follows (4,5)
		Follows (5,6)
		Follows (6,10)
		Follows (7,8)
		Follows (8,9)
		Follows (10,13)
		Follows (13,14)
		Follows (14,15)
		-
		Follows (16,17)

		Uses (6,0)
		Uses (7,2)
		Uses (7,3)
		Uses (9,0)
		Uses (10,2)
		Uses (11,2)
		Uses (13,4)
		Uses (13,2)
		Uses (13,0)
		Uses (14,4)
		Uses (15,2)
		Uses (15,3)
		Uses (15,4)
		-
		Uses (17,4)

		Modifies (1,2)
		Modifies (2,4)
		-
		Modifies (4,2)
		Modifies (5,0)
		Modifies (7,2)
		Modifies (9,0)
		Modifies (11,2)
		Modifies (12,4)
		Modifies (13,4)
		Modifies (14,3)
		Modifies (15,2)
		-
		Modifies (16,4)
		Modifies (17,1)

		UsesP (1,0)
		UsesP (1,2)
		UsesP (1,3)
		UsesP (1,4)
		-
		UsesP (2,4)

		ModifiesP (0,2)
		ModifiesP (0,4)
		-
		ModifiesP (1,0)
		ModifiesP (1,2)
		ModifiesP (1,3)
		ModifiesP (1,4)
		-
		ModifiesP (2,1)
		ModifiesP (2,4)

		Parent (6,7)
		Parent (6,8)
		Parent (6,9)
		Parent (10,11)
		Parent (10,12)

		Next (1,2)
		Next (2,3)
		-
		Next (4,5)
		Next (5,6)
		Next (6,7)
		Next (7,8)
		Next (8,9)
		Next (9,6)
		Next (6,10)
		Next (10,11)
		Next (11,13)
		Next (12,13)
		Next (13,14)
		Next (14,15)
		-
		Next (16,17)

		Calls (0,1)
		Calls (1,2)
*/

DummyPKB::DummyPKB()
{
}

ProcIndex DummyPKB::getProcIndex(ProcName procName)
{
	if (procName == "First")
		return 0;
	if (procName == "Second")
		return 1;
	if (procName == "Third")
		return 2;

	return -1;
}

ProcName DummyPKB::getProcName(ProcIndex procIndex)
{
	if (procIndex == 0)
		return "First";
	if (procIndex == 1)
		return "Second";
	if (procIndex == 2)
		return "Third";

	return "";
}

VarIndex DummyPKB::getVarIndex(VarName varName) 
{
	if (varName == "i")
		return 0;
	if (varName == "v")
		return 1;
	if (varName == "x")
		return 2;
	if (varName == "y")
		return 3;
	if (varName == "z")
		return 4;
	
	return -1;
}

VarName DummyPKB::getVarName(VarIndex varIndex) {
	if (varIndex == 0)
		return "i";
	if (varIndex == 1)
		return "v";
	if (varIndex == 2)
		return "x";
	if (varIndex == 3)
		return "y";
	if (varIndex == 4)
		return "z";

	return "";
}

bool DummyPKB::isProcExist(ProcName procName)
{
	if (procName == "First" || procName == "Second" || procName =="Third")
		return true;
	
	return false;
}

bool DummyPKB::isStmtExist(StmtNumber stmt)
{
	std::set<StmtNumber> statements = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };
	if (statements.count(stmt))
		return true;

	return false;
}

bool DummyPKB::isVarExist(VarName varName)
{
	if (varName == "i" || varName == "v" || varName == "x" || varName == "y" || varName == "z")
		return true;

	return false;
}

bool DummyPKB::is(RelationshipType rel, StmtNumber stmt, ProcStmtVarIndex item)
{
	if (rel == FOLLOWS) {
		if (stmt == 1 && item == 2)
			return true;
		if (stmt == 2 && item == 3)
			return true;
		if (stmt == 4 && item == 5)
			return true;
		if (stmt == 5 && item == 6)
			return true;
		if (stmt == 6 && item == 10)
			return true;
		if (stmt == 7 && item == 8)
			return true;
		if (stmt == 8 && item == 9)
			return true;
		if (stmt == 10 && item == 13)
			return true;
		if (stmt == 13 && item == 14)
			return true;
		if (stmt == 14 && item == 15)
			return true;
		if (stmt == 16 && item == 17)
			return true;
	}
	
	else if (rel == USES) {
		if (stmt == 6 && item == 0)
			return true;
		if (stmt == 7 && item == 2)
			return true;
		if (stmt == 7 && item == 3)
			return true;
		if (stmt == 9 && item == 0)
			return true;
		if (stmt == 10 && item == 2)
			return true;
		if (stmt == 11 && item == 2)
			return true;
		if (stmt == 13 && item == 4)
			return true;
		if (stmt == 13 && item == 2)
			return true;
		if (stmt == 13 && item == 0)
			return true;
		if (stmt == 14 && item == 4)
			return true;
		if (stmt == 15 && item == 2)
			return true;
		if (stmt == 15 && item == 3)
			return true;
		if (stmt == 15 && item == 4)
			return true;
		if (stmt == 17 && item == 4)
			return true;
	}

	else if (rel == MODIFIES) {
		if (stmt == 1 && item == 2)
			return true;
		if (stmt == 2 && item == 4)
			return true;
		if (stmt == 4 && item == 2)
			return true;
		if (stmt == 5 && item == 0)
			return true;
		if (stmt == 7 && item == 2)
			return true;
		if (stmt == 9 && item == 0)
			return true;
		if (stmt == 11 && item == 2)
			return true;
		if (stmt == 12 && item == 4)
			return true;
		if (stmt == 13 && item == 4)
			return true;
		if (stmt == 14 && item == 3)
			return true;
		if (stmt == 15 && item == 2)
			return true;
		if (stmt == 16 && item == 4)
			return true;
		if (stmt == 17 && item == 1)
			return true;
	}

	else if (rel == USES_P) {
		if (stmt == 1 && item == 0)
			return true;
		if (stmt == 1 && item == 2)
			return true;
		if (stmt == 1 && item == 3)
			return true;
		if (stmt == 1 && item == 4)
			return true;
		if (stmt == 2 && item == 4)
			return true;
	}

	else if (rel == MODIFIES_P) {
		if (stmt == 0 && item == 2)
			return true;
		if (stmt == 0 && item == 4)
			return true;
		if (stmt == 1 && item == 0)
			return true;
		if (stmt == 1 && item == 2)
			return true;
		if (stmt == 1 && item == 3)
			return true;
		if (stmt == 1 && item == 4)
			return true;
		if (stmt == 2 && item == 1)
			return true;
		if (stmt == 2 && item == 4)
			return true;
	}

	else if (rel == PARENT) {
		if (stmt == 6 && item == 7)
			return true;
		if (stmt == 6 && item == 8)
			return true;
		if (stmt == 6 && item == 9)
			return true;
		if (stmt == 10 && item == 11)
			return true;
		if (stmt == 10 && item == 12)
			return true;
	}

	else if (rel == NEXT) {
		if (stmt == 1 && item == 2)
			return true;
		if (stmt == 2 && item == 3)
			return true;
		if (stmt == 4 && item == 5)
			return true;
		if (stmt == 5 && item == 6)
			return true;
		if (stmt == 6 && item == 7)
			return true;
		if (stmt == 7 && item == 8)
			return true;
		if (stmt == 8 && item == 9)
			return true;
		if (stmt == 9 && item == 6)
			return true;
		if (stmt == 6 && item == 10)
			return true;
		if (stmt == 10 && item == 11)
			return true;
		if (stmt == 11 && item == 13)
			return true;
		if (stmt == 12 && item == 13)
			return true;
		if (stmt == 13 && item == 14)
			return true;
		if (stmt == 14 && item == 15)
			return true;
		if (stmt == 16 && item == 17)
			return true;
	}

	else if (rel == CALLS) {
		if (stmt == 0 && item == 1)
			return true;
		if (stmt == 1 && item == 2)
			return true;
	}

	return false;
}

bool DummyPKB::isAssignHasExpr(StmtNumber assign, StringToken expr)
{
	return false;
}

bool DummyPKB::isAssignHasSubexpr(StmtNumber assign, StringToken expr)
{
	// pattern a(8,"_z_"), a(5,_3_)
	return false;
}

bool DummyPKB::isAssignExactPattern(StmtNumber, InfixExpr)
{
	return false;
}

bool DummyPKB::isAssignContainsPattern(StmtNumber, InfixExpr)
{
	return false;
}

bool DummyPKB::isIfPattern(StmtNumber ifStmt, VarIndex varIndex)
{
	if (ifStmt == 10 && varIndex == 2)
		return true;

	return false;
}

bool DummyPKB::isWhilePattern(StmtNumber whileStmt, VarIndex varIndex)
{
	if (whileStmt == 6 && varIndex == 0)
		return true;

	return false;
}

std::set<StmtNumber> DummyPKB::getAllStmts()
{
	std::set<StmtNumber> stmts = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };
	return stmts;
}

std::set<VarIndex> DummyPKB::getAllVarIndex() {
    return std::set<VarIndex>({ 0,1,2,3,4 });
}

std::set<VarName> DummyPKB::getAllVarNames()
{
	// variables : x,y,w,z
	std::set<VarName> vars = { "i", "v", "x","y","z" };
	return vars;
}

std::set<ProcIndex> DummyPKB::getAllProcIndex()
{
	return std::set<ProcIndex>({ 0,1,2 });
}

std::set<ProcName> DummyPKB::getAllProcNames()
{
	return std::set<ProcName>({ "First", "Second", "Third" });
}

std::set<Constant> DummyPKB::getAllConstantValues()
{
	return std::set<Constant>({ 0,1,2,3,5 });
}

std::set<StmtNumber> DummyPKB::getStmtsByType(EntityType stmtType)
{
	if (stmtType == ASSIGN) {
		std::set<StmtNumber> assigns = { 1,2,4,5,7,9,11,12,13,14,15,16,17 };
		return assigns;
	}
	else if (stmtType == STMT || stmtType == PROGRAM_LINE) {
		std::set<StmtNumber> stmts = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };
		return stmts;
	}
	else if (stmtType == WHILE) {
		std::set<StmtNumber> whiles = { 6 };
		return whiles;
	}
	else if (stmtType == IF) {
		std::set<StmtNumber> ifs = { 10 };
		return ifs;
	}
	else if (stmtType == CALL) {
		std::set<StmtNumber> calls = { 3,8 };
		return calls;
	}
	else if (stmtType == PROCEDURE) {
		std::set<ProcIndex> procedures = { 0,1,2 };
		return procedures;
	}
	else if (stmtType == CONSTANT) {
		std::set<Constant> constants = { 0,1,2,3,5 };
		return constants;
	}

	return std::set<StmtNumber>();
}

// ** Not used anymore **
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

std::set<VarIndex> DummyPKB::getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses)
{
	// (3,modifies) =  Modifies(3,v)
	if (modifiesOrUses == MODIFIES) {
		if (stmt == 1)
			return std::set<VarIndex>({ 2 });
		if (stmt == 2)
			return std::set<VarIndex>({ 4 });
		if (stmt == 4)
			return std::set<VarIndex>({ 2 });
		if (stmt == 5)
			return std::set<VarIndex>({ 0 });
		if (stmt == 7)
			return std::set<VarIndex>({ 2 });
		if (stmt == 9)
			return std::set<VarIndex>({ 0 });
		if (stmt == 11)
			return std::set<VarIndex>({ 2 });
		if (stmt == 12)
			return std::set<VarIndex>({ 4 });
		if (stmt == 13)
			return std::set<VarIndex>({ 4 });
		if (stmt == 14)
			return std::set<VarIndex>({ 3 });
		if (stmt == 15)
			return std::set<VarIndex>({ 2 });
		if (stmt == 16)
			return std::set<VarIndex>({ 4 });
		if (stmt == 17)
			return std::set<VarIndex>({ 1 });

		return std::set<VarIndex>({});
	}
	else if (modifiesOrUses == USES) {
		if (stmt == 6)
			return std::set<VarIndex>({ 0 });
		if (stmt == 7)
			return std::set<VarIndex>({ 2,3 });
		if (stmt == 9)
			return std::set<VarIndex>({ 0 });
		if (stmt == 10)
			return std::set<VarIndex>({ 2 });
		if (stmt == 11)
			return std::set<VarIndex>({ 2 });
		if (stmt == 13)
			return std::set<VarIndex>({ 0,2,4 });
		if (stmt == 14)
			return std::set<VarIndex>({ 4 });
		if (stmt == 15)
			return std::set<VarIndex>({ 2,3,4 });
		if (stmt == 17)
			return std::set<VarIndex>({ 4 });

		return std::set<VarIndex>({});
	}

	return std::set<VarIndex>();
}

ProcIndex DummyPKB::getProcByCall(StmtNumber callStmt)
{
	if (callStmt == 3)
		return 1;
	if (callStmt == 8)
		return 2;

	return -1;
}

std::set<ProcIndex> DummyPKB::getProcsByProc(RelationshipType calls, ProcIndex procIndex)
{
	if (procIndex == 0) {
		return std::set<ProcIndex>({ 1 });
	}
	if (procIndex == 1) {
		return std::set<ProcIndex>({ 2 });
	}

	return std::set<ProcIndex>({});
}

std::set<ProcIndex> DummyPKB::getProcsByProc(ProcIndex procIndex, RelationshipType calls)
{
	if (procIndex == 1) {
		return std::set<ProcIndex>({ 0 });
	}
	if (procIndex == 2) {
		return std::set<ProcIndex>({ 1 });
	}
	return std::set<ProcIndex>({});
}

std::set<VarIndex> DummyPKB::getVarsByProc(ProcIndex procIndex, RelationshipType modifiesOrUses)
{
	if (modifiesOrUses == MODIFIES_P) {
		if (procIndex == 0) {
			return std::set<VarIndex>({2,4});
		}
		if (procIndex == 1) {
			return std::set<VarIndex>({0,2,3,4});
		}
		if (procIndex == 0) {
			return std::set<VarIndex>({1,4});
		}
	}

	else if (modifiesOrUses == USES_P) {
		if (procIndex == 0) {
			return std::set<VarIndex>({});
		}
		if (procIndex == 1) {
			return std::set<VarIndex>({ 0,2,3,4 });
		}
		if (procIndex == 0) {
			return std::set<VarIndex>({ 4 });
		}
	}

	return std::set<VarIndex>();
}

std::set<ProcStmtVarIndex> DummyPKB::getStmtsByStmt(StmtNumber stmt, RelationshipType followsOrParent)
{
	//	Argument : (3, FOLLOWS) ===> Follows(s,3)
	// Get all statements of left synonym

	if (followsOrParent == FOLLOWS) {
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 3) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 5) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
		if (stmt == 6) {
			std::set<StmtNumber> cases = { 5 };
			return cases;
		}
		if (stmt == 10) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 8) {
			std::set<StmtNumber> cases = { 7 };
			return cases;
		}
		if (stmt == 9) {
			std::set<StmtNumber> cases = { 8 };
			return cases;
		}
		if (stmt == 13) {
			std::set<StmtNumber> cases = { 10 };
			return cases;
		}
		if (stmt == 14) {
			std::set<StmtNumber> cases = { 13 };
			return cases;
		}
		if (stmt == 15) {
			std::set<StmtNumber> cases = { 14 };
			return cases;
		}
		if (stmt == 17) {
			std::set<StmtNumber> cases = { 16 };
			return cases;
		}
	}

	else if (followsOrParent == USES) {
		if (stmt == 0) {
			std::set<StmtNumber> cases = { 6,9,13 };
			return cases;
		}
		if (stmt == 1) {
			std::set<StmtNumber> cases = {};
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 7,10,11,13,15 };
			return cases;
		}
		if (stmt == 3) {
			std::set<StmtNumber> cases = { 7,15 };
			return cases;
		}
		if (stmt == 4) {
			std::set<StmtNumber> cases = { 13,14,15,17 };
			return cases;
		}
	}

	else if (followsOrParent == MODIFIES) {
		if (stmt == 0) {
			std::set<StmtNumber> cases = { 5,9 };
			return cases;
		}
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 17 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 1,4,7,11,15 };
			return cases;
		}
		if (stmt == 3) {
			std::set<StmtNumber> cases = { 14 };
			return cases;
		}
		if (stmt == 4) {
			std::set<StmtNumber> cases = { 12,13,16 };
			return cases;
		}
	}

	else if (followsOrParent == USES_P) {
		if (stmt == 0) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 1) {
			std::set<StmtNumber> cases = { };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 3) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 4) {
			std::set<StmtNumber> cases = { 1,2 };
			return cases;
		}
	}

	else if (followsOrParent == MODIFIES_P) {
		if (stmt == 0) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 0,1 };
			return cases;
		}
		if (stmt == 3) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 4) {
			std::set<StmtNumber> cases = { 0,1,2 };
			return cases;
		}
	}

	else if (followsOrParent == PARENT) {
		if (stmt == 7) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 8) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 9) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 11) {
			std::set<StmtNumber> cases = { 10 };
			return cases;
		}
		if (stmt == 12) {
			std::set<StmtNumber> cases = { 10 };
			return cases;
		}
	}

	else if (followsOrParent == NEXT) {
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 3) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 5) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
		if (stmt == 6) {
			std::set<StmtNumber> cases = { 5,9 };
			return cases;
		}
		if (stmt == 7) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 8) {
			std::set<StmtNumber> cases = { 7 };
			return cases;
		}
		if (stmt == 9) {
			std::set<StmtNumber> cases = { 8 };
			return cases;
		}
		if (stmt == 10) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 11) {
			std::set<StmtNumber> cases = { 10 };
			return cases;
		}
		if (stmt == 13) {
			std::set<StmtNumber> cases = { 11,12 };
			return cases;
		}
		if (stmt == 14) {
			std::set<StmtNumber> cases = { 13 };
			return cases;
		}
		if (stmt == 15) {
			std::set<StmtNumber> cases = { 14 };
			return cases;
		}
		if (stmt == 17) {
			std::set<StmtNumber> cases = { 16 };
			return cases;
		}
	}

	else if (followsOrParent == CALLS) {
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 0 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
	}

	return std::set<StmtNumber>();
}

std::set<StmtNumber> DummyPKB::getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt)
{
	//	Argument : (FOLLOWS, 3) ===> Follows(3,s)
	// Get all statements of right synonym
	if (followsOrParent == FOLLOWS) {
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 3 };
			return cases;
		}
		if (stmt == 4) {
			std::set<StmtNumber> cases = { 5 };
			return cases;
		}
		if (stmt == 5) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 6) {
			std::set<StmtNumber> cases = { 10 };
			return cases;
		}
		if (stmt == 7) {
			std::set<StmtNumber> cases = { 8 };
			return cases;
		}
		if (stmt == 8) {
			std::set<StmtNumber> cases = { 9 };
			return cases;
		}
		if (stmt == 10) {
			std::set<StmtNumber> cases = { 13 };
			return cases;
		}
		if (stmt == 13) {
			std::set<StmtNumber> cases = { 14 };
			return cases;
		}
		if (stmt == 14) {
			std::set<StmtNumber> cases = { 15 };
			return cases;
		}
		if (stmt == 16) {
			std::set<StmtNumber> cases = { 17 };
			return cases;
		}
	}

	else if (followsOrParent == USES) {
		if (stmt == 6) {
			std::set<StmtNumber> cases = { 0 };
			return cases;
		}
		if (stmt == 7) {
			std::set<StmtNumber> cases = { 2,3 };
			return cases;
		}
		if (stmt == 9) {
			std::set<StmtNumber> cases = { 0 };
			return cases;
		}
		if (stmt == 10) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 11) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 13) {
			std::set<StmtNumber> cases = { 0,2,4 };
			return cases;
		}
		if (stmt == 14) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
		if (stmt == 15) {
			std::set<StmtNumber> cases = { 2,3,4 };
			return cases;
		}
		if (stmt == 17) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
	}

	else if (followsOrParent == MODIFIES) {
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
		if (stmt == 4) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 5) {
			std::set<StmtNumber> cases = { 0 };
			return cases;
		}
		if (stmt == 7) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 9) {
			std::set<StmtNumber> cases = { 0 };
			return cases;
		}
		if (stmt == 11) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 12) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
		if (stmt == 13) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
		if (stmt == 14) {
			std::set<StmtNumber> cases = { 3 };
			return cases;
		}
		if (stmt == 15) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 16) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
		if (stmt == 17) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
	}

	else if (followsOrParent == USES_P) {
		if (stmt == 0) {
			std::set<StmtNumber> cases = {};
			return cases;
		}
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 0,2,3,4 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 4 };
			return cases;
		}
	}

	else if (followsOrParent == MODIFIES_P) {
		if (stmt == 0) {
			std::set<StmtNumber> cases = { 2,4 };
			return cases;
		}
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 0,2,3,4 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 1,4 };
			return cases;
		}
	}

	else if (followsOrParent == PARENT) {
		if (stmt == 6) {
			std::set<StmtNumber> cases = { 7,8,9 };
			return cases;
		}
		if (stmt == 10) {
			std::set<StmtNumber> cases = { 11,12 };
			return cases;
		}
	}

	else if (followsOrParent == NEXT) {
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
		if (stmt == 2) {
			std::set<StmtNumber> cases = { 3 };
			return cases;
		}
		if (stmt == 4) {
			std::set<StmtNumber> cases = { 5 };
			return cases;
		}
		if (stmt == 5) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 6) {
			std::set<StmtNumber> cases = { 7,10 };
			return cases;
		}
		if (stmt == 7) {
			std::set<StmtNumber> cases = { 8 };
			return cases;
		}
		if (stmt == 8) {
			std::set<StmtNumber> cases = { 9 };
			return cases;
		}
		if (stmt == 9) {
			std::set<StmtNumber> cases = { 6 };
			return cases;
		}
		if (stmt == 10) {
			std::set<StmtNumber> cases = { 11 };
			return cases;
		}
		if (stmt == 11) {
			std::set<StmtNumber> cases = { 13 };
			return cases;
		}
		if (stmt == 12) {
			std::set<StmtNumber> cases = { 13 };
			return cases;
		}
		if (stmt == 13) {
			std::set<StmtNumber> cases = { 14 };
			return cases;
		}
		if (stmt == 14) {
			std::set<StmtNumber> cases = { 15 };
			return cases;
		}
		if (stmt == 16) {
			std::set<StmtNumber> cases = { 17 };
			return cases;
		}
	}

	else if (followsOrParent == CALLS) {
		if (stmt == 0) {
			std::set<StmtNumber> cases = { 1 };
			return cases;
		}
		if (stmt == 1) {
			std::set<StmtNumber> cases = { 2 };
			return cases;
		}
	}
	
	return std::set<StmtNumber>();
}
