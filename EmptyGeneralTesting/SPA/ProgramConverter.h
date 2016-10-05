#pragma once

#include <cctype>
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
public:
	ProgramConverter();
	int convert(std::string source);

	bool isVarName(std::string str);
	bool isConstant(std::string str);
	bool isAssignment(ProgLine line);
	bool isWhile(ProgLine line);
	bool isIf(ProgLine line);
	bool isElse(ProgLine line);
	bool isCall(ProgLine line);
	bool isEnterParent(std::string str);
	bool isExitParent(std::string str);
	bool isLineEnding(std::string str);
	bool updateAssignmentInAssignmentTrees(ProgLine line, ProgLineNumber lineNum);
	bool updateAssignmentInTable(ProgLine line, ProgLineNumber lineNum);
	bool updateStmtInStmtTable(ProgLine line, ProgLineNumber lineNum);
	
};
