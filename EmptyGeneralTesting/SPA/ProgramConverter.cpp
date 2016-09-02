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

void ProgramConverter::convert(std::string source) {
	StringTokenizer st = StringTokenizer(source, DelimiterMode::PARSER);
	ProgLineNumber numLines = 0;

	for (ProgLineNumber lineNum = 1; lineNum < numLines; lineNum++) {
		ProgLine currentLine;

		updateStmtInStmtTable(currentLine, lineNum);                 // Aaron

		if (isAssignment(currentLine)) {
			updateAssignmentInAssignmentTrees(currentLine, lineNum); // Ngoc Khanh
			updateAssignmentInVarTable(currentLine, lineNum);        // Kai Lin
		}
	}
}

// Aaron
bool ProgramConverter::isAssignment(ProgLine line) {
	return false;
}

// Ngoc Khanh
bool ProgramConverter::updateAssignmentInAssignmentTrees(ProgLine line, ProgLineNumber lineNum) {
	AssignTree tree = AssignTree();
	pkb->putAssign(lineNum, tree);

	return false;
}

// Kai Lin
bool ProgramConverter::updateAssignmentInVarTable(ProgLine line, ProgLineNumber lineNum) {
	VarName varName = "varName";
	VarIndex varIndex = pkb->getVarIndex(varName);

	pkb->putVar(lineNum, MODIFIES, varIndex);
	pkb->putVar(lineNum, USES, varIndex);

	return false;
}

// Aaron
bool ProgramConverter::updateStmtInStmtTable(ProgLine line, ProgLineNumber lineNum) {
	return false;
}
