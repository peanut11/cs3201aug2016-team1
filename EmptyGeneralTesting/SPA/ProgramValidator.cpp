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
				throw std::runtime_error("Invalid syntax");
			}
		}
	} else {
		throw std::runtime_error("Invalid syntax");
	}

	return true;
}

bool ProgramValidator::isMatch(std::string actual, std::string expected) {
	bool isIgnoreNewlines = true;

	if (isIgnoreNewlines) {
		if (actual == "}" && expected == "\n") {
			// st.returnToken(actual);
			// return true;

		} else if (actual == "\n" && expected != "\n") {
			return isMatch(st.nextToken(), expected);
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
	return isAssign(str) || isWhile(str);
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

bool ProgramValidator::isAssign(std::string str) {
		// Note: 'while' is a valid varName if followed by '=' instead of another varName
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

bool ProgramValidator::isConstValue(std::string str) {
	return isInteger(str);
}
