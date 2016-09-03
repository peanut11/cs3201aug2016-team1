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
	st = StringTokenizer(source, DelimiterMode::PARSER);
	ProgLine currentLine;

	while (!(currentLine = nextLine()).empty()) {
		ProgLineNumber lineNum = lineCount;
		updateStmtInStmtTable(currentLine, lineNum);                 // Aaron

		if (isAssignment(currentLine)) {
			updateAssignmentInAssignmentTrees(currentLine, lineNum); // Ngoc Khanh
			updateAssignmentInVarTable(currentLine, lineNum);        // Kai Lin
		}
	}
}

// Returns:     The next ProgLine if exist, or an empty ProgLine.
// Guarantees:  If the return is not empty, lineCount == lineNum.
// Consequence: If the return is empty, it is end of the Program.
ProgLine ProgramConverter::nextLine() {
	if (!st.hasMoreTokens()) {
		return ProgLine();
	}

	ProgLine line;
	std::string token = st.nextToken();

	while (isLineEnding(token) && st.hasMoreTokens()) {
		token = st.nextToken();
	}

	st.returnToken(token);
	
	while (st.hasMoreTokens() && !isLineEnding(token = st.nextToken())) {
		line.push_back(token);
	}

	if (line.empty()) {
		return nextLine();
	}

	lineCount++;

	return line;
}

bool ProgramConverter::isAssignment(ProgLine line) {
	return line[2] == "=";
}

bool ProgramConverter::isLineEnding(std::string str) {
	const std::string LINE_ENDINGS = "{;}\n"; 
	char ch = str[0];

	return LINE_ENDINGS.find(ch) != std::string::npos;
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
