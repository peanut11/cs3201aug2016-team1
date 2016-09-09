// Maintained by: Ngoc Khanh
//
// Accessed by:
// - Parser
//
// Constructs:
// - AssignTree

#include "ProgramConverter.h"

bool ProgramConverter::isVarName(std::string str) {
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

bool ProgramConverter::isConstant(std::string str)
{
	if (str.empty()) {
		return false;
		
	}
	for (unsigned int i = 0; i < str.length(); i++) {
		if (!std::isdigit(str.at(i))) {
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

int ProgramConverter::convert(std::string source) {
	st = StringTokenizer(source, DelimiterMode::PARSER);
	lineCount = 0;
	ProgLine currentLine;

	while (!(currentLine = nextLine()).empty()) {
		const std::string FIRST_TOKEN = currentLine[0];

		if (FIRST_TOKEN == "procedure") {
			currentLeader = 0;
			currentParent = 0;
			continue;
		}

		if (isEnterParent(FIRST_TOKEN)) {
			currentLeader = 0;
			currentParent = lineCount;
			continue;
		}

		if (isExitParent(FIRST_TOKEN)) {
			currentLeader = currentParent;
			std::set<StmtNumber> parentSet = pkb->getStmtsByStmt(currentParent, PARENT);

			if (parentSet.empty()) {
				currentParent = 0;
			} else {
				StmtSetIterator it = parentSet.begin();
				currentParent = *it;
			}

			continue;
		}

		const ProgLineNumber lineNum = lineCount;
		updateStmtInStmtTable(currentLine, lineNum);
	}

	return lineCount;
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
			break;
		} else {
			line.push_back(token);
		}
	}

	if (line.empty()) {
		return nextLine();
	}
	
	if (line[0] != "procedure") {
		lineCount++;
	}

	return line;
}

bool ProgramConverter::isAssignment(ProgLine line) {
	const std::string SECOND_TOKEN = line[1];
	return SECOND_TOKEN == "=";
}

bool ProgramConverter::isWhile(ProgLine line) {
	const std::string FIRST_TOKEN = line[0];
	return FIRST_TOKEN == "while";
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

bool ProgramConverter::updateAssignmentInAssignmentTrees(ProgLine line, ProgLineNumber lineNum) {
	const AssignTree tree = AssignTree(line);
	pkb->putAssignForStmt(lineNum, tree);

	return false;
}

bool ProgramConverter::updateAssignmentInTable(ProgLine line, ProgLineNumber lineNum) {
	bool isRHS = false;
	bool res = true;

	for each (std::string str in line) {
		if (isVarName(str)) {
			const VarName varName = str;

			if (isRHS) {
				res = pkb->putVarForStmt(lineNum, USES, varName);
			} else {
				res = pkb->putVarForStmt(lineNum, MODIFIES, varName);
			}
			
			if (!res) return res; // Returns immediately if false

		} else if (isConstant(str)) {
			Constant constant = atoi(str.c_str());
			res = pkb->putConstant(constant);
		} else {
			if (str == "=") isRHS = true; // Ignores the rest of the signs
		}
	}
	
	return res;
}

bool ProgramConverter::updateStmtInStmtTable(ProgLine line, ProgLineNumber lineNum) {
	bool success = true;

	if (isAssignment(line)) {
		success = pkb->putStmtTypeForStmt(lineNum, ASSIGN) && success;
		success = updateAssignmentInTable(line, lineNum) && success;
		success = updateAssignmentInAssignmentTrees(line, lineNum) && success;

	} else if (isWhile(line)) {
		success = pkb->putStmtTypeForStmt(lineNum, WHILE) && success;

		const VarName varName = line[1];
		success = pkb->putVarForStmt(lineNum, USES, varName) && success;
	}

	if (currentParent != 0) {
		success = pkb->putStmtForStmt(lineNum, PARENT, currentParent) && success;
	}

	if (currentLeader != 0) {
		success = pkb->putStmtForStmt(lineNum, FOLLOWS, currentLeader) && success;
	}

	currentLeader = lineNum;

	return success;
}