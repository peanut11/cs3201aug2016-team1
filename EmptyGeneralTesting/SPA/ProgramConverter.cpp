// Maintained by: Ngoc Khanh
//
// Accessed by:
// - Parser
//
// Constructs:
// - AssignTree

#include "ProgramConverter.h"
#include <string>
#include <cctype>

bool ProgramConverter::isVarName(std::string str)
{
	if (str.empty()) {
		return false;
	}
	
	if (!std::isalpha(str.at(0))) {
		return false;
	}
	for (unsigned int i = 1; i < str.length(); i++) {
		if (!std::isalnum(str.at(i))) {
			return false;
		}
	}
	return true;
}

ProgramConverter::ProgramConverter() {
	pkb = PKB::getInstance();
	currentLeader = 0;
	currentParent = 0;
	lineCount = 0;
}

void ProgramConverter::convert(std::string source) {
	st = StringTokenizer(source, DelimiterMode::PARSER);
	ProgLine currentLine;

	while (!(currentLine = nextLine()).empty()) {
		const std::string FIRST_TOKEN = currentLine[0];

		if (isEnterParent(FIRST_TOKEN)) {
			currentLeader = 0; 
			currentParent = lineCount;
			continue;

		} else if (isExitParent(FIRST_TOKEN)) {
			currentLeader = currentParent;
			std::vector<StmtNumber> parentVec = pkb->getStmts(currentParent, PARENT);

			if (parentVec.empty()) {
				currentParent = 0;
			} else {
				currentParent = parentVec[0];
			}
			continue;
		}

		ProgLineNumber lineNum = lineCount;
		updateStmtInStmtTable(currentLine, lineNum);                 // Aaron

		if (isAssignment(currentLine)) {
			updateAssignmentInAssignmentTrees(currentLine, lineNum); // Ngoc Khanh
			updateAssignmentInVarTable(currentLine, lineNum);        // Kai Lin
		}
	}
}

// Possible return values:
// 1. The next ProgLine if it exists.
// 2. A ProgLine containing either only "{" or "}".
// 3. An empty ProgLine, which implies end of the Program.
//
// Guarantees:
// 1. lineCount == lineNum.
// 2. lineCount is not incremented.
// 3. lineCount is not incremented.
ProgLine ProgramConverter::nextLine() {
	if (!st.hasMoreTokens()) {
		return ProgLine();
	}

	ProgLine line;
	std::string token = st.nextToken();

	while (isLineEnding(token) && st.hasMoreTokens()) {
		token = st.nextToken();
	}

	if (isEnterParent(token) || isExitParent(token)) {
		line.push_back(token);
		return line;
	}

	st.returnToken(token);
	
	while (st.hasMoreTokens() && !isLineEnding(token = st.nextToken())) {
		if (isEnterParent(token) || isExitParent(token)) {
			st.returnToken(token);
			return line;
		}

		line.push_back(token);
	}

	if (line.empty()) {
		return nextLine();
	}

	lineCount++;

	return line;
}

bool ProgramConverter::isAssignment(ProgLine line) {
	const std::string SECOND_TOKEN = line[1];
	return SECOND_TOKEN == "=";
}

bool ProgramConverter::isEnterParent(std::string str) {
	return str == "{";
}

bool ProgramConverter::isExitParent(std::string str) {
	return str == "}";
}

bool ProgramConverter::isLineEnding(std::string str) {
	const std::string LINE_ENDINGS = ";\n";
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
	bool isRHS = false;
	bool res = true;
	for each (std::string str in line)
	{
		if (isVarName(str)) {
			VarName varName = str;
			VarIndex varIndex = pkb->getVarIndex(varName); 
			
			// assumes will deifnitely get a valid index

			if (isRHS) {
				res = pkb->putVar(lineNum, USES, varIndex);
			}
			else {
				res = pkb->putVar(lineNum, MODIFIES, varIndex);
			}
			
			if (!res) return res; //returns immediately if false
		}
		else { // assumes string '='
			isRHS = true;
		}
	}
	
	return res;
}

bool ProgramConverter::updateStmtInStmtTable(ProgLine line, ProgLineNumber lineNum) {
	bool success;

	if (currentParent != 0) {
		success = pkb->putStmt(lineNum, PARENT, currentParent);
	}

	if (currentLeader != 0) {
		success = pkb->putStmt(lineNum, FOLLOWS, currentLeader) && success;
	}

	currentLeader = lineNum;

	return success;
}
