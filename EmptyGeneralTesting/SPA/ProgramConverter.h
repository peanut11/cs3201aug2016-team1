#pragma once

#include <string>

#include "AssignTree.h"
#include "PKB.h"
#include "StringTokenizer.h"

class ProgramConverter {
private:
	PKB* pkb;

public:
	ProgramConverter();
	void convert(std::string);
	bool isAssignment(std::vector<std::string> line);
	bool updateAssignmentInAssignmentTrees(std::vector<std::string> line, int lineNum);
	bool updateAssignmentInVarTable(std::vector<std::string> line, int lineNum);
	bool updateStmtInStmtTable(std::vector<std::string> line, int lineNum);
};
