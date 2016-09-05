// Maintained by: Kai Lin

#pragma once

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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
typedef std::vector<StmtNumber> VarEntry;
typedef std::vector<VarOrStmt> StmtEntry;
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

	bool contains(std::vector<VarOrStmt> vec, VarOrStmt item);

public:
	static PKB* getInstance();

	// Indexing
	VarIndex getVarIndex(VarName varName); //gets index of variable and adds  to tables if variable not found
	VarName getVarName(VarIndex varIndex);

	// API used by Parser
	bool putAssignForStmt(StmtNumber stmt, AssignTree tree);
	bool putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB);
	bool putVarForStmt(StmtNumber stmt, RelationshipType rel, VarName varName);
	bool putConstant(Constant constant);

	// API used by QP
	bool is(RelationshipType rel, StmtNumber stmtA, StmtNumber stmtB); // E.g. is(FOLLOWS,1,2)
	bool isVarExist(VarName varName);

	AssignTree              getAssign(StmtNumber stmt);
	StmtNumber              getStmtTableSize();
	std::vector<Constant>   getAllConstantValues();
	std::vector<StmtNumber> getAllStmts();
	std::vector<VarName>    getAllVarNames();
	std::vector<StmtNumber> getStmtsByType(EntityType stmtType);
	std::vector<StmtNumber> getStmtsByVar(RelationshipType rel, VarName varName);
	std::vector<StmtNumber> getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel);
	std::vector<VarIndex>   getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses);
};
