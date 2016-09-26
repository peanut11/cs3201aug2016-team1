#pragma once

#include <cctype>
#include <string>

#include "StringTokenizer.h"

class ProgramValidator {
private:
	StringTokenizer st;

	bool isMatch(std::string actual, std::string expected);
	bool isName(std::string str);
	bool isInteger(std::string str);

    bool isProgram(std::string str);
	bool isProcedure(std::string str);
	bool isStmtLst(std::string str);
	bool isStmt(std::string str);
    bool isCall(std::string str);
	bool isWhile(std::string str);
    bool isIf(std::string str);
	bool isAssign(std::string str);
	bool isExpr(std::string str);
	bool isFactor(std::string str);
	bool isVarName(std::string str);
    bool isProcName(std::string str);
	bool isConstValue(std::string str);

public:
	ProgramValidator();

	bool isValidSyntax(std::string program);
};
