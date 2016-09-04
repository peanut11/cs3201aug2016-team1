#pragma once

#include <string>

#include "AssignTree.h"
#include "PKB.h"
#include "StringTokenizer.h"

class ProgramConverter {
private:
	PKB* pkb;
	StringTokenizer st;
	ProgLineNumber currentParent;
	ProgLineNumber currentLeader; 
	ProgLineNumber lineCount;
	ProgLine nextLine();
	bool isVarName(std::string str);
	bool isAssignment(ProgLine line);
	bool isEnterParent(std::string str);
	bool isExitParent(std::string str);
	bool isLineEnding(std::string str);
	bool updateAssignmentInAssignmentTrees(ProgLine line, ProgLineNumber lineNum);
	bool updateAssignmentInVarTable(ProgLine line, ProgLineNumber lineNum);
	bool updateStmtInStmtTable(ProgLine line, ProgLineNumber lineNum);

public:
	ProgramConverter();
	void convert(std::string source);
};
