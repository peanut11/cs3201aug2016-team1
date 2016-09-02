// Maintained by: Kai Lin

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "AssignTree.h"
#include "EntityType.h"
#include "RelationshipType.h"

typedef int CONST; 
typedef int PROC_INDEX; // Index of procedure name in ProcTable
typedef int STMT_NUM; 
typedef int VAR_INDEX;  // Index of variable name in VarTable and constant value
typedef std::string VAR_NAME; 
typedef std::map<VAR_NAME, VAR_INDEX> REF_TABLE;
typedef std::vector<std::vector<STMT_NUM>[2]> VAR_TABLE;
typedef std::vector<std::vector<VAR_INDEX>[RelationshipType::TOTAL_COUNT]> STMT_TABLE;

class PKB {
private:
	static const std::runtime_error ERROR;
	static PKB* theOne;
	PKB();
	
	std::vector<AssignTree> assignTrees;
	REF_TABLE refTable; 
	STMT_TABLE stmtTable;
	std::vector<EntityType> stmtTypeTable;
	VAR_TABLE varTable;
	std::vector<CONST> constants;

public:
	static PKB* getInstance();

	std::vector<CONST> getAllConstantValues(); 
	std::vector<VAR_NAME> getAllVarNames();
	AssignTree getAssign(STMT_NUM stmt);
	std::vector<STMT_NUM> getStmts(VAR_INDEX var, RelationshipType rel);
	std::vector<VAR_INDEX> getVars(STMT_NUM stmt, RelationshipType relIsMU); // MU: ModifiesUses
	std::vector<STMT_NUM> getStmts(RelationshipType relNotMU, STMT_NUM stmt);
	std::vector<STMT_NUM> getStmts(EntityType stmtType);

	bool putVar(STMT_NUM dest, RelationshipType rel, VAR_INDEX var);
	bool putStmt(STMT_NUM dest, RelationshipType rel, STMT_NUM stmt);
	bool putAssign(STMT_NUM dest, AssignTree tree);
};
