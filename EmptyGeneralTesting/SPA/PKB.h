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
typedef std::vector<VarOrStmt> StmtRow[RelationshipType::TOTAL_COUNT];
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
	VarIndex getVarIndex(VarName varName);
	VarName getVarName(VarIndex varIndex);

	// API used by Parser
	bool putAssign(StmtNumber dest, AssignTree tree); 
	bool putStmt(StmtNumber dest, RelationshipType rel, StmtNumber stmt);
	bool putVar(StmtNumber dest, RelationshipType rel, VarIndex varIndex);

	// API used by QP
	bool is(RelationshipType rel, StmtNumber stmtA, StmtNumber stmtB); // E.g. is(FOLLOWS,1,2)
	bool is(StmtNumber stmt, RelationshipType rel, VarOrStmt item);    // E.g. is(1,MODIFIES,2)
	AssignTree              getAssign(StmtNumber stmt);
	std::vector<Constant>   getAllConstantValues();
	std::vector<VarName>    getAllVarNames();
	std::vector<StmtNumber> getStmts(EntityType stmtType);
	std::vector<StmtNumber> getStmts(RelationshipType rel, VarIndex varIndex);
	std::vector<StmtNumber> getStmts(StmtNumber stmt, RelationshipType stmtRel);
	std::vector<VarIndex>   getVars(StmtNumber stmt, RelationshipType modifiesOrUses);
};
