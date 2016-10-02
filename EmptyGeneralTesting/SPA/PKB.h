// Maintained by: Kai Lin

#pragma once

#include <string>
#include <vector>
#include <set>

#include "AssignTree.h"
#include "EntityType.h"
#include "Exceptions.h"
#include "RelationshipType.h"
#include "Types.h"

class PKB {
private:
	static PKB* theOne;
	
	std::vector<AssignTree>				assignTrees;
	std::set<Constant>					constants;
	VarRefMap							varRefMap;
	std::vector<VarName>				varRefTable;
	std::vector<VarRow>					varTable;
	std::vector<StmtRow>				stmtTable;
	std::vector<EntityType>				stmtTypeTable;
	std::vector<StmtSet>				stmtByTypeTable;
	ProcRefMap							procRefMap;
	std::vector<ProcName>				procRefTable;
	std::vector<ProcRow>				procTable;
	std::vector<ProcIndex>				stmtToProcTable;
	std::vector<std::set<StmtNumber>>	procToStmtTable;


public:
	static PKB* getInstance();
	void clear();
	PKB();
	// Indexing
	virtual VarIndex getVarIndex(VarName varName); //gets index of variable and adds to tables if variable not found
	virtual VarName getVarName(VarIndex varIndex);
	virtual ProcIndex getProcIndex(ProcName procName);
	virtual ProcName getProcName(ProcIndex procIndex);

	// API used by Parser and DE
	bool putAssignForStmt(StmtNumber stmt, AssignTree tree);
	bool putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB);
	bool putStmtTypeForStmt(StmtNumber stmt, EntityType stmtType);
	bool putVarForStmt(StmtNumber stmt, RelationshipType modifiesOrUses, VarName varName); //updates MODIFIES and USES relationships in Vartable and StmtTable
	bool putConstant(Constant constant);
	bool putStmtProc(StmtNumber stmt, ProcName procNameContainingStmt);
	bool putProcForProc(ProcName procA, RelationshipType calls, ProcName procB);

	// API used by QP and DE
	/*
	Follows( 1, 2) -> bool result = is(FOLLOWS,1,2);
	Follows(s1, 2) -> std::vector<StmtNumber> s1 = getStmtsByStmt(2,FOLLOWS);     // Get stmts that stmt 2 follows
	Follows( 1,s2) -> std::vector<StmtNumber> s2 = getStmtsByStmt(1,FOLLOWED_BY); // Get stmts that stmt 1 is followed by
	Follows(s1,s2) -> std::vector<StmtNumber> s1 = s2 = getAllStmts();
	Follows( a,s2) -> std::vector<StmtNumber>  a = getStmtsByType(ASSIGN);
					  std::vector<StmtNumber> s2 = getAllStmts();
					  for (int i = 0; i < a.size(); i++) {
						for (int j = 0; j < s2.size(); j++) {
						  if (is(FOLLOWS,a,s2)) {
						    if (query == "Select a") {
							  results.append(a);
							} else if (query == "Select s2") {
							  results.append(s2);
							}
						  }
						}
					  }
	*/
	virtual bool is(RelationshipType rel, StmtNumber stmt, StmtVarIndex item);
	virtual bool isAssignHasExpr(StmtNumber assign, ExprString expr);
	virtual bool isAssignHasSubexpr(StmtNumber assign, ExprString subexpr);
	bool isVarExist(VarName varName);

	virtual AssignTree              getAssign(StmtNumber stmt);
	virtual EntityType				getStmtTypeForStmt(StmtNumber stmt);
	virtual StmtNumber              getStmtTableSize();
	virtual std::set<Constant>		getAllConstantValues();
	virtual std::set<StmtNumber>	getAllStmts();
    virtual std::set<VarIndex>		getAllVarIndex();
	virtual std::set<VarName>		getAllVarNames();
	virtual ProcIndex				getProcByStmt(StmtNumber stmt);

	//getting statements
	virtual std::set<StmtNumber>	getAllStmts();
	virtual std::set<StmtNumber>	getStmtsByType(EntityType stmtType);
	virtual std::set<StmtNumber>	getStmtsByProc(ProcName procName);
	virtual std::set<StmtNumber>	getStmtsByVar(RelationshipType modifiesOrUses, VarName varName);
	virtual std::set<StmtNumber>	getStmtsByStmt(StmtNumber stmt, RelationshipType followsOrParent);
	virtual std::set<StmtNumber>	getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt);
	//getting variables
	virtual std::set<VarIndex>		getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses);
	virtual std::set<VarIndex>		getVarsByProc(ProcName procName, RelationshipType modifiesOrUses);
	//getting procedures
	virtual std::set<ProcIndex>		getProcsByProc(ProcName procName, RelationshipType calls);
	virtual std::set<ProcIndex>		getProcsByVar(RelationshipType modifiesOrUses, VarName varName);
};
