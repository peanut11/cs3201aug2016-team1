// Maintained by: Ngoc Khanh
//
// Accessed by:
// - Parser
//
// Constructs:
// - AssignTree

#include "ProgramConverter.h"

ProgramConverter::ProgramConverter() {
	pkb = PKB::getInstance();
}

void ProgramConverter::convert(std::string program) {
	StringTokenizer st = StringTokenizer(program, DelimiterMode::PARSER);
	int numLines = 0;

	for (int lineNum = 1; lineNum < numLines; lineNum++) {
		std::vector<std::string> currentLine;

		updateStmtInStmtTable(currentLine, lineNum);                 // Aaron

		if (isAssignment(currentLine)) {
			updateAssignmentInAssignmentTrees(currentLine, lineNum); // Ngoc Khanh
			updateAssignmentInVarTable(currentLine, lineNum);        // Kai Lin
		}
	}
}

// Aaron
bool ProgramConverter::isAssignment(std::vector<std::string> line) {
	return false;
}

// Ngoc Khanh
bool ProgramConverter::updateAssignmentInAssignmentTrees(std::vector<std::string> line, int lineNum) {
	AssignTree tree = AssignTree();
	pkb->putAssign(lineNum, tree);

	return false;
}

// Kai Lin
bool ProgramConverter::updateAssignmentInVarTable(std::vector<std::string> line, int lineNum) {
	VAR_INDEX var = 0;
	pkb->putVar(lineNum, MODIFIES, var);
	pkb->putVar(lineNum, USES, var);

	return false;
}

// Aaron
bool ProgramConverter::updateStmtInStmtTable(std::vector<std::string> line, int lineNum) {
	return false;
}
