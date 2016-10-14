#pragma once
#include "PKB.h"
#include <string>
#include <vector>
#include <set>
#include "EntityType.h"
#include "RelationshipType.h"
#include "Types.h"

class DummyPKB : public PKB {

public:
	DummyPKB();

	virtual ProcIndex getProcIndex(ProcName procName);
	virtual ProcName  getProcName(ProcIndex procIndex);
	virtual VarIndex getVarIndex(VarName varName); //gets index of variable and adds  to tables if variable not found
	virtual VarName getVarName(VarIndex varIndex);

	virtual bool isProcExist(ProcName procName);
	virtual bool isStmtExist(StmtNumber stmt);
	virtual bool isVarExist(VarName varName);
	virtual bool is(RelationshipType rel, StmtNumber stmt, ProcStmtVarIndex item);
	virtual bool isAssignHasExpr(StmtNumber assign, StringToken);	// Not used
	virtual bool isAssignHasSubexpr(StmtNumber assign, StringToken);	// Not used
	virtual bool isAssignExactPattern(StmtNumber, InfixExpr);
	virtual bool isAssignContainsPattern(StmtNumber, InfixExpr);
	virtual bool isIfPattern(StmtNumber ifStmt, VarIndex varIndex);
	virtual bool isWhilePattern(StmtNumber whileStmt, VarIndex varIndex);

	virtual std::set<StmtNumber>	getAllStmts();
    virtual std::set<VarIndex>      getAllVarIndex();
	virtual std::set<VarName>       getAllVarNames();
	virtual std::set<ProcIndex>		getAllProcIndex();
	virtual std::set<ProcName>		getAllProcNames();
	virtual std::set<Constant>		getAllConstantValues();

	virtual std::set<StmtNumber>	getStmtsByType(EntityType stmtType);
	virtual std::set<StmtNumber>	getStmtsByVar(RelationshipType modifiesOrUses, VarName varName);
	virtual std::set<StmtNumber>	getStmtsByStmt(StmtNumber stmt, RelationshipType followsOrParent);
	virtual std::set<StmtNumber>	getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt);
	virtual std::set<VarIndex>		getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses);
	
	virtual ProcIndex				getProcByCall(StmtNumber callStmt);
	virtual std::set<ProcIndex>		getProcsByProc(RelationshipType calls, ProcIndex procIndex);
	virtual std::set<ProcIndex>		getProcsByProc(ProcIndex procIndex, RelationshipType calls);
	virtual std::set<VarIndex>		getVarsByProc(ProcIndex procIndex, RelationshipType modifiesOrUses);

};
