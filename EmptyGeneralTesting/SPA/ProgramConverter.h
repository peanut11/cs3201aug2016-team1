#pragma once

#include <string>

#include "AssignTree.h"
#include "PKB.h"
#include "StringTokenizer.h"

class ProgramConverter {
private:
	PKB* pkb;

	bool isAssignment(ProgLine line);
	bool updateAssignmentInAssignmentTrees(ProgLine line, ProgLineNumber lineNum);
	bool updateAssignmentInVarTable(ProgLine line, ProgLineNumber lineNum);
	bool updateStmtInStmtTable(ProgLine line, ProgLineNumber lineNum);

public:
	ProgramConverter();
	void convert(std::string source);
};
