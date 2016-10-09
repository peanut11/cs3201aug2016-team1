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

	virtual VarIndex getVarIndex(VarName varName); //gets index of variable and adds  to tables if variable not found
	virtual VarName getVarName(VarIndex varIndex);

	virtual bool is(RelationshipType rel, StmtNumber stmt, ProcStmtVarIndex item);
	virtual bool isAssignHasExpr(StmtNumber assign, StringToken);
	virtual bool isAssignHasSubexpr(StmtNumber assign, StringToken);
	virtual std::set<StmtNumber>	getAllStmts();
    virtual std::set<VarIndex>      getAllVarIndex();
	virtual std::set<VarName>       getAllVarNames();
	virtual std::set<StmtNumber>	getStmtsByType(EntityType stmtType);
	virtual std::set<StmtNumber>	getStmtsByVar(RelationshipType modifiesOrUses, VarName varName);
	virtual std::set<StmtNumber>	getStmtsByStmt(StmtNumber stmt, RelationshipType followsOrParent);
	virtual std::set<StmtNumber>	getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt);
	virtual std::set<VarIndex>		getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses);

};