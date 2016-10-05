// Maintained by: Aaron
//
// Accessed by:
// - Parser

#include "ProgramValidator.h"

ProgramValidator::ProgramValidator() {}

bool ProgramValidator::isValidSyntax(std::string str) {
	st = StringTokenizer(str, DelimiterMode::PARSER);

	if (!st.hasMoreTokens() || !isProgram(st.nextToken())) {
        throw Exception::INVALID_SIMPLE_SYNTAX;
	}

    return true;
}

void ProgramValidator::registerCalled(std::string procName) {
    procAdjList[currentProcedure].push_back(procName);

    if (procedures.find(procName) == procedures.end()) {
        unknownCalls.push(procName);
    }
}

bool ProgramValidator::registerProcedure(std::string procName) {
    bool success = procedures.emplace(procName).second;

    if (!success) {
        throw Exception::DUPLICATE_PROCEDURE_NAME;
    }

    return true;
}

bool ProgramValidator::hasNoInvalidCalls() {
    while (!unknownCalls.empty()) {
        std::string procName = unknownCalls.front();
        unknownCalls.pop();

        if (procedures.find(procName) == procedures.end()) {
            throw Exception::INVALID_CALL_MADE;
            return false;
        }
    }

    return true;
}

void ProgramValidator::discardNewlines() {
    while (isMatch(st.peekNextToken(), "\n")) {
        st.popNextToken();
    }
}

bool ProgramValidator::isMatch(std::string actual, std::string expected) {
	if (st.getIsIgnoreNewlines()) {
        if (expected == "\n") {
            st.returnToken(actual);
            return true;
        }
	}

	return actual == expected;
}

bool ProgramValidator::isName(std::string str) {
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

bool ProgramValidator::isInteger(std::string str) {
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

bool ProgramValidator::isNotRecursiveCall(std::string procName) {
    if (breadthFirstSearch(procName)) {
        throw Exception::RECURSIVE_CALL_ERROR;
    }

    return true;
}

bool ProgramValidator::breadthFirstSearch(std::string procName) {
    std::set<std::string> traversed;
    std::queue<std::string> toVisit;

    traversed.emplace(procName);
    toVisit.push(procName);

    while (!toVisit.empty()) {
        std::string parent = toVisit.front();
        toVisit.pop();

        std::vector<std::string> children = procAdjList[parent];
        for (unsigned int i = 0; i < children.size(); i++) {
            std::string child = children[i];

            if (child == currentProcedure) {
                return true;
            } else if (traversed.find(child) == traversed.end()) {
                traversed.emplace(child);
                toVisit.push(child);
            }
        }
    }

    return false;
}

bool ProgramValidator::isProgram(std::string str) {
    while (isProcedure(str)) {
        if (st.hasMoreTokens() && !isMatch(st.nextToken(), "\n")) {
            return false;
        }

        discardNewlines();

        if (st.hasMoreTokens()) {
            str = st.nextToken();
        }
    }

    return !st.hasMoreTokens() && hasNoInvalidCalls();
}

bool ProgramValidator::isProcedure(std::string str) {
	return isMatch(str, "procedure")
		&& isName(currentProcedure = st.nextToken()) && registerProcedure(currentProcedure)
		&& isMatch(st.nextToken(), "{")
		&& isMatch(st.nextToken(), "\n")
		&& isStmtLst(st.nextToken())
		&& isMatch(st.nextToken(), "}");
}

bool ProgramValidator::isStmtLst(std::string str) {
	if (isStmt(str)) {
		while (isMatch(st.peekNextToken(), "\n")) {
			st.popNextToken();
			const std::string token = st.nextToken();
			if (!isStmt(token)) {
				st.returnToken(token);
			}
		}
		return true;
	}

	return false;
}

bool ProgramValidator::isStmt(std::string str) {
	return isAssign(str) || isCall(str) || isIf(str) || isWhile(str);
}

bool ProgramValidator::isCall(std::string str) {
    // Note: 'call' is a valid varName if followed by '=' instead of another varName
    if (isMatch(str, "call") && isProcName(st.peekNextToken())) {
        std::string calledProcedure = st.nextToken();
        registerCalled(calledProcedure);

        return isMatch(st.nextToken(), ";") && isNotRecursiveCall(calledProcedure);
    }
    
    return false;
}

bool ProgramValidator::isWhile(std::string str) {
	// Note: 'while' is a valid varName if followed by '=' instead of another varName
	if (isMatch(str, "while") && isVarName(st.peekNextToken())) {
		st.popNextToken();
		return isMatch(st.nextToken(), "{")
			&& isMatch(st.nextToken(), "\n")
			&& isStmtLst(st.nextToken())
			&& isMatch(st.nextToken(), "}");
	}

	return false;
}

bool ProgramValidator::isIf(std::string str) {
    // Note: 'if' is a valid varName if followed by '=' instead of another varName
    if (isMatch(str, "if") && isVarName(st.peekNextToken())) {
        st.popNextToken();
        return isMatch(st.nextToken(), "then")
            && isMatch(st.nextToken(), "{")
            && isMatch(st.nextToken(), "\n")
            && isStmtLst(st.nextToken())
            && isMatch(st.nextToken(), "}")
            && isMatch(st.nextToken(), "\n")
            && isMatch(st.nextToken(), "else")
            && isMatch(st.nextToken(), "{")
            && isMatch(st.nextToken(), "\n")
            && isStmtLst(st.nextToken())
            && isMatch(st.nextToken(), "}");
    }

    return false;
}

bool ProgramValidator::isAssign(std::string str) {
    // Note: 'call/if/while' are valid varNames if followed by '=' instead of another varName
	if (isVarName(str) && isMatch(st.peekNextToken(), "=")) {
		st.popNextToken();
		return isExpr(st.nextToken())
			&& isMatch(st.nextToken(), ";");
	}

	return false;
}

bool ProgramValidator::isExpr(std::string str) {
    bool hasNoParentheses = true;
    bool isExpression = false;

    if (isMatch(str, "(")) {
        hasNoParentheses = false;
        str = st.nextToken();
    }

	if (isFactor(str)) {
        isExpression = true;
        str = st.peekNextToken();
	}

    if (isExpression && ( isMatch(str, "+") || isMatch(str, "-") || isMatch(str, "*") )) {
        st.popNextToken();
        isExpression = isExpr(st.nextToken());
    }

	return isExpression && (hasNoParentheses || isMatch(st.nextToken(), ")"));;
}

bool ProgramValidator::isFactor(std::string str) {
	return isVarName(str) || isConstValue(str);
}

bool ProgramValidator::isVarName(std::string str) {
	return isName(str);
}

bool ProgramValidator::isProcName(std::string str) {
    return isName(str);
}

bool ProgramValidator::isConstValue(std::string str) {
	return isInteger(str);
}
