#pragma once

#include <cassert>
#include <cctype>
#include <string>

#include "PKB.h"
#include "StringTokenizer.h"

class ProgramConverter {
private:
	PKB* pkb;
	StringTokenizer st;
	ProgLineNumber previous;
	ProgLineNumber currentParent;
	ProgLineNumber currentLeader;
	ProgLineNumber lineCount;
	ProgLine nextLine();

    std::map<ProgLineNumber, std::array<ProgLineNumber,2>> lastOfIfLists;
    
    bool setNext(ProgLineNumber prev, ProgLineNumber next);
    bool updateAssignmentInPostfixExprs(ProgLine, ProgLineNumber);
    bool updateAssignmentInTable(ProgLine line, ProgLineNumber lineNum);
    bool updateNextforBasedon(ProgLineNumber stmt, ProgLineNumber leader);
    bool updateParentAndPrevious(ProgLineNumber lineNum);
    bool updateStmtInStmtTable(ProgLine line, ProgLineNumber lineNum);

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
};
