#pragma once

#include <cctype>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "Exceptions.h"
#include "StringTokenizer.h"

class ProgramValidator {
private:
	StringTokenizer st;
    std::string currentProcedure;
    std::map<std::string, std::vector<std::string>> procAdjList;
    std::set<std::string> procedures;
    std::queue<std::string> unknownCalls;

    void registerCalled(std::string procName); // Only queues if unknown procedure
    bool registerProcedure(std::string procName); // Checks if duplicate procedure
    bool hasNoInvalidCalls(); // Checks if any of the unknownCalls are invalid

    void discardNewlines();
	bool isMatch(std::string actual, std::string expected);
	bool isName(std::string str);
	bool isInteger(std::string str);
    bool isNotRecursiveCall(std::string procName);
    bool breadthFirstSearch(std::string procName);

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
