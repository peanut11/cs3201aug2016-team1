// Maintained by: Aaron
//
// Accessed by:
// - Parser

#include "ProgramValidator.h"

ProgramValidator::ProgramValidator() {}

bool ProgramValidator::isValidSyntax(std::string str) {
	st = StringTokenizer(str, DelimiterMode::PARSER);
	const std::string token = st.nextToken();

	if (isProcedure(token)) {
		while (st.hasMoreTokens()) {
			if (!isMatch(st.nextToken(), "\n")) {
                Exception::INVALID_SIMPLE_SYNTAX;
			}
		}
	} else {
        throw Exception::INVALID_SIMPLE_SYNTAX;
	}

	return true;
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

bool ProgramValidator::isProgram(std::string str) {
    bool isProgram = false;

    while (st.hasMoreTokens()) {
        while (isMatch(st.peekNextToken(), "\n")) {
            st.popNextToken();
        }

        if (st.hasMoreTokens()) {
            isProgram = isProcedure(st.nextToken());
        }
    }
    return isProgram;
}

bool ProgramValidator::isProcedure(std::string str) {
	return isMatch(str, "procedure")
		&& isName(st.nextToken())
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
	return isAssign(str) || isWhile(str) || isCall(str);
}

bool ProgramValidator::isCall(std::string str) {
    // Note: 'call' is a valid varName if followed by '=' instead of another varName
    if (isMatch(str, "call") && isProcName(st.peekNextToken())) {
        std::string calledProcedure = st.nextToken();
        procAdjList[currentProcedure].push_back(calledProcedure);

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
	if (isFactor(str)) {
		if (isMatch(st.peekNextToken(), "+")) {
			st.popNextToken();
			return isExpr(st.nextToken());
		}
		return true;
	}
	return false;
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
