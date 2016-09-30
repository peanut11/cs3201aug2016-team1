#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>
#include <set>

#include "RelationshipType.h"

typedef int Constant;
typedef std::string StringToken;
typedef std::vector<StringToken> ProgLine;
typedef StringToken ExprString;

typedef unsigned int ProcStmtVarIndex;
typedef ProcStmtVarIndex SynonymValue; 
typedef ProcStmtVarIndex StmtVarIndex;
typedef StmtVarIndex ProcIndex;  // Index of procedure name in ProcTable
typedef StmtVarIndex StmtNumber;
typedef StmtVarIndex VarIndex;   // Index of variable/constant in VarTable
typedef StmtNumber ProgLineNumber;
typedef std::set<StmtNumber> StmtSet;
typedef StmtSet::const_iterator StmtSetIterator;

typedef StringToken VarName;
typedef VarName SynonymString; 
typedef std::map<VarName, VarIndex> RefMap;
typedef std::set<VarIndex>::const_iterator VarIndexSetIterator;

typedef std::set<StmtNumber> VarEntry;
typedef std::array<VarEntry, 2> VarRow; 

typedef std::set<StmtVarIndex> StmtEntry;
typedef std::array<StmtEntry, RelationshipType::TOTAL_COUNT> StmtRow; 
