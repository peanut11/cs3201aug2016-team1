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
class DesignExtractor {
private:
	std::vector<StmtRow>    stmtTable;
public:
	void DesignExtractor::process();
	void DesignExtractor::processFollowStar();
	void DesignExtractor::processParentStar();
	void DesignExtractor::updateStmtTable();
	void DesignExtractor::processWhileLoop(StmtNumber i);
};
