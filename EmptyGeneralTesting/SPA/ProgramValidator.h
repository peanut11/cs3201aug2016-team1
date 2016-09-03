#pragma once

#include <stack>
#include <string>
#include "StringTokenizer.h"

class ProgramValidator {
	StringTokenizer st;
	bool isValidSyntax();
	bool isMatch(std::string s1, std::string s2);

	bool isName(std::string str);
	bool isInteger(std::string str);
	bool isProcedure(std::string str);
	bool isStmtLst(std::string str);
	bool isStmt(std::string str);
	bool isWhile(std::string str);
	bool isAssign(std::string str);
	bool isExpr(std::string str);
	bool isFactor(std::string str);
	bool isVarName(std::string str);
	bool isConstValue(std::string str);

public:
	ProgramValidator();
	bool isValidProgram(std::string str);
};