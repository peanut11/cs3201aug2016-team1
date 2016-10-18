// Maintained by: Ngoc Khanh
//
// Accessed by:
// - Parser
//
// Constructs:
// - AssignTree

#include <cassert>
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
	ProcName procName;
	bool isElse = false;
	bool isStmtLst = false;
	ProgLineNumber lastOfThenList = 0;

	while (!(currentLine = nextLine()).empty()) {
		const std::string FIRST_TOKEN = currentLine[0];

		if (FIRST_TOKEN == "procedure") {
			currentLeader = 0; 
			currentParent = 0;
			previous = 0;
			procName = currentLine[1];
			continue;
		}

		if (FIRST_TOKEN == "else") {
			isElse = true;
			lastOfThenList = lineCount;
			continue;
		}

		if (!isElse && lastOfThenList != 0) {
			pkb->putStmtForStmt(lastOfThenList, NEXT, lineCount);
			lastOfThenList = 0;
		}

		if (isEnterParent(FIRST_TOKEN)) {
			isStmtLst = true;
			if (isElse) {
				currentLeader = 0;
				std::set<StmtNumber> parentSet = pkb->getStmtsByStmt(lineCount, PARENT);

				if (parentSet.empty()) {
					throw std::runtime_error("");
				} else {
					currentParent = *parentSet.begin();

					while (pkb->getStmtTypeForStmt(currentParent) == WHILE) {
						StmtSet parentSet = pkb->getStmtsByStmt(currentParent, PARENT);
						currentParent = *parentSet.begin();
					}

					if (pkb->getStmtTypeForStmt(currentParent) == IF) {
						previous = currentParent;
					} else {
						throw std::runtime_error("");
					}
				}
			} else {
				currentLeader = 0;
				currentParent = lineCount;
			}
			continue;
		}

		if (isExitParent(FIRST_TOKEN)) {
			currentLeader = currentParent;
			std::set<StmtNumber> parentSet = pkb->getStmtsByStmt(currentParent, PARENT);
			if (pkb->getStmtTypeForStmt(currentParent) == WHILE) {
				pkb->putStmtForStmt(previous, NEXT, currentParent);
				previous = currentParent;
			}
	
			if (parentSet.empty()) {
				currentParent = 0;
			} else {
				StmtSetIterator it = parentSet.begin();
				currentParent = *it;
				
			}
			
			if (isElse) isElse = false;
			continue;
		}

		const ProgLineNumber lineNum = lineCount;
		if (isStmtLst) {
			pkb->putStmtLst(lineNum);
			isStmtLst = false;
		}
		pkb->putStmtProc(lineNum, procName);
		updateStmtInStmtTable(currentLine, lineNum);
		currentLeader = lineNum;
		previous = currentLeader;
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
	
	if (line[0] != "procedure" && line[0] != "else") {
		lineCount++;
	}

	return line;
}

bool ProgramConverter::isAssignment(ProgLine line) {
	if (line.size() >= 3) {
		const std::string SECOND_TOKEN = line[1];
		return SECOND_TOKEN == "=";
	} else {
		return false;
	}
	
}

bool ProgramConverter::isWhile(ProgLine line) {
	const std::string FIRST_TOKEN = line[0];
	return FIRST_TOKEN == "while";
}

bool ProgramConverter::isIf(ProgLine line) {
	const std::string FIRST_TOKEN = line[0];
	return FIRST_TOKEN == "if";
}

bool ProgramConverter::isElse(ProgLine line) {
	const std::string FIRST_TOKEN = line[0];
	return FIRST_TOKEN == "else";
}

bool ProgramConverter::isCall(ProgLine line) {
	const std::string FIRST_TOKEN = line[0];
	return FIRST_TOKEN == "call";
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
//	const AssignTree tree = AssignTree(line);
//	pkb->putAssignForStmt(lineNum, tree);

	return false;
}

bool ProgramConverter::updateAssignmentInPostfixExprs(ProgLine line, ProgLineNumber lineNum) {
	assert(line.size() > 2);
	assert(line[1] == "=");

	if (line.back() == ";") line.pop_back();

	InfixExpr infix = InfixExpr (line.begin()+2, line.end());

	const PostfixExpr postfix = pkb->infixToPostfix(infix);
	return pkb->putExprForStmt(lineNum, postfix);
}

bool ProgramConverter::updateAssignmentInTable(ProgLine line, ProgLineNumber lineNum) {
	bool isRHS = false;
	bool res = true;
	ProcName procName = pkb->getProcName(pkb->getProcByStmt(lineNum));
	for each (std::string str in line) {
		if (isConstant(str)) {
			Constant constant = atoi(str.c_str());
			res = pkb->putConstant(constant);
		} else if (isVarName(str)) {
			const VarName varName = str;

			if (isRHS) {
				res = pkb->putVarForStmt(lineNum, USES, varName);
				res = pkb->putVarForProc(procName, USES, varName);
			} else {
				res = pkb->putVarForStmt(lineNum, MODIFIES, varName);
				res = pkb->putVarForProc(procName, MODIFIES, varName);
			}
			
			if (!res) return res; // Returns immediately if false

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
		success = updateAssignmentInPostfixExprs(line, lineNum) && success;
	} else if (isWhile(line)) {
		success = pkb->putStmtTypeForStmt(lineNum, WHILE) && success;
		
		const VarName varName = line[1];
		success = pkb->putControlVarForStmt(lineNum, varName) && success;
		success = pkb->putVarForStmt(lineNum, USES, varName) && success;
		success = pkb->putVarForProc(pkb->getProcName(pkb->getProcByStmt(lineNum)), USES, varName) && success;
	} else if (isIf(line)) {
		success = pkb->putStmtTypeForStmt(lineNum, IF) && success;

		const VarName varName = line[1];
		success = pkb->putControlVarForStmt(lineNum, varName) && success;
		success = pkb->putVarForStmt(lineNum, USES, varName) && success;
		success = pkb->putVarForProc(pkb->getProcName(pkb->getProcByStmt(lineNum)), USES, varName) && success;
	} else if (isCall(line)) {
		ProcName procCalled = line[1];
		success = pkb->putStmtTypeForStmt(lineNum, CALL) && success;
		success = pkb->putStmtCallProc(lineNum, procCalled) && success;
		success = pkb->putProcForProc(pkb->getProcByStmt(lineNum), CALLS, procCalled) && success;
	}

	if (currentParent != 0) {
		success = pkb->putStmtForStmt(currentParent, PARENT, lineNum) && success;
	}

	if (currentLeader != 0) {
		success = pkb->putStmtForStmt(currentLeader, FOLLOWS, lineNum) && success;
	}

	if (previous != 0) {
		success = pkb->putStmtForStmt(previous, NEXT, lineNum) && success;
	}

	return success;
}
