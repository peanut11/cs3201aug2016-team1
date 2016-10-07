#pragma once

#include <array>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "RelationshipType.h"

typedef int Constant;
typedef std::string StringToken;
typedef std::vector<StringToken> ProgLine;
typedef std::vector<StringToken> InfixExpr;
typedef std::vector<StringToken> PostfixExpr;
typedef StringToken ExprString;

typedef unsigned int ProcStmtVarIndex; // Entry in StmtTable
typedef ProcStmtVarIndex SynonymValue;  
typedef ProcStmtVarIndex StmtNumber;
typedef ProcStmtVarIndex VarIndex;
typedef ProcStmtVarIndex ProcIndex;
typedef ProcStmtVarIndex ProcVarIndex; // Entry in ProcTable
typedef ProcVarIndex ProcIndex;
typedef ProcVarIndex VarIndex;  
typedef ProcStmtVarIndex ProcStmtIndex; //Entry in VarTable
typedef ProcStmtIndex ProcIndex;
typedef ProcStmtIndex StmtNumber;

typedef StmtNumber ProgLineNumber;
typedef std::set<StmtNumber> StmtSet;
typedef StmtSet::const_iterator StmtSetIterator;

typedef StringToken VarName;
typedef VarName SynonymString; 
typedef std::map<VarName, VarIndex> VarRefMap;
typedef std::set<VarIndex>::const_iterator VarIndexSetIterator;

// VarTable
typedef std::set<ProcStmtIndex> VarEntry;
typedef std::array<VarEntry, 4> VarRow; // ModifiedByStmt, UsedByStmt, ModifiedByProc, UsedByProc

// StmtTable
typedef std::set<ProcStmtVarIndex> StmtEntry;
typedef std::array<StmtEntry, RelationshipType::TOTAL_COUNT> StmtRow; 

// ProcRefTable
typedef StringToken ProcName;
typedef std::map<ProcName, ProcIndex> ProcRefMap; 

// ProcTable
typedef std::set<ProcVarIndex> ProcEntry;
typedef std::array<ProcEntry, 6> ProcRow; // Modifies, Uses, Calls, Called_By, Calls_Star, Called_By_Star
