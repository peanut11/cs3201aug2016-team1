// Maintained by: Kai Lin

#pragma once

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

#include "AssignTree.h"
#include "EntityType.h"
#include "RelationshipType.h"

typedef int Constant; 
typedef int ProcIndex;        // Index of procedure name in ProcTable
typedef unsigned int VarOrStmt;
typedef VarOrStmt ProgLineNumber;
typedef VarOrStmt VarIndex;   // Index of variable/constant in VarTable
typedef VarOrStmt StmtNumber; // StmtNumber == ProgLineNumber
typedef std::string VarName;
typedef std::map<VarName, VarIndex> RefMap;
typedef std::vector<std::string> ProgLine;
typedef std::set<StmtNumber> VarEntry;
typedef std::set<VarOrStmt> StmtEntry;
typedef std::array<StmtEntry, RelationshipType::TOTAL_COUNT> StmtRow;
typedef std::array<VarEntry, 2> VarRow;

class PKB {
private:
	static const std::runtime_error ERROR;
	static PKB* theOne;
	PKB();
	
	std::vector<AssignTree> assignTrees;
	std::vector<Constant>   constants;
	RefMap                  refMap;
	std::vector<VarName>    refTable;
	std::vector<StmtRow>    stmtTable;
	std::vector<EntityType> stmtTypeTable;
	std::vector<VarRow>     varTable;

public:
	static PKB* getInstance();

	// Indexing
	VarIndex getVarIndex(VarName varName); //gets index of variable and adds  to tables if variable not found
	VarName getVarName(VarIndex varIndex);

	// API used by Parser and DE
	bool putAssignForStmt(StmtNumber stmt, AssignTree tree);
	bool putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB);
	bool putStmtTypeForStmt(StmtNumber stmt, EntityType stmtType);
	bool putVarForStmt(StmtNumber stmt, RelationshipType rel, VarName varName);
	bool putConstant(Constant constant);

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
	bool is(RelationshipType rel, StmtNumber stmt, VarOrStmt item);
	bool isVarExist(VarName varName);

	AssignTree              getAssign(StmtNumber stmt);
	EntityType				getStmtTypeForStmt(StmtNumber stmt);
	StmtNumber              getStmtTableSize();
	std::vector<Constant>   getAllConstantValues();
	std::set<StmtNumber>	getAllStmts();
	std::vector<VarName>    getAllVarNames();
	std::set<StmtNumber>	getStmtsByType(EntityType stmtType);
	std::set<StmtNumber>	getStmtsByVar(RelationshipType rel, VarName varName);
	std::set<StmtNumber>	getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel);
	std::set<VarIndex>		getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses);
};
