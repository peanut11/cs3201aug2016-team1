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
typedef int ProcIndex; // Index of procedure name in ProcTable
typedef int ProgLineNumber;
typedef int VarIndex;  // Index of variable name in VarTable and constant value
typedef unsigned int StmtNumber;
typedef std::string VarName;
typedef std::map<VarName, VarIndex> RefMap;
typedef std::vector<std::string> ProgLine;
typedef std::vector<StmtNumber> VarEntry;
typedef std::vector<VarIndex> StmtVarRow[2];
typedef std::vector<StmtNumber> StmtStmtRow[RelationshipType::TOTAL_COUNT-2];
typedef std::array<VarEntry, 2> VarRow;

class PKB {
private:
	static const std::runtime_error ERROR;
	static PKB* theOne;
	PKB();
	
	std::vector<AssignTree>  assignTrees;
	std::vector<Constant>    constants;
	RefMap                   refMap;
	std::vector<VarName>     refTable;
	std::vector<StmtVarRow>  stmtVarTable;
	std::vector<StmtStmtRow> stmtStmtTable;
	std::vector<EntityType>  stmtTypeTable;
	std::vector<VarRow>      varTable;

public:
	static PKB* getInstance();

	std::vector<Constant>   getAllConstantValues();
	std::vector<VarName>    getAllVarNames(); 
	AssignTree              getAssign(StmtNumber stmt);
	std::vector<StmtNumber> getStmts(RelationshipType rel, VarIndex varIndex);
	std::vector<StmtNumber> getStmts(StmtNumber stmt, RelationshipType relNotMU);
	std::vector<StmtNumber> getStmts(EntityType stmtType);
	VarIndex                getVarIndex(VarName varName);
	VarName                 getVarName(VarIndex varIndex);
	std::vector<VarIndex>   getVars(StmtNumber stmt, RelationshipType relIsMU); // MU: ModifiesUses

	bool putVar(StmtNumber dest, RelationshipType rel, VarIndex varIndex);
	bool putStmt(StmtNumber dest, RelationshipType rel, StmtNumber stmt);
	bool putAssign(StmtNumber dest, AssignTree tree);
};
