#include <cctype>
#include "QueryValidator.h"

const std::string QueryValidator::SYNTAX_PROCEDURE = "procedure";
const std::string QueryValidator::SYNTAX_SELECT = "select";

QueryValidator *QueryValidator::_instance;

/*
Singleton
*/
QueryValidator *QueryValidator::getInstance()
{
	if (!_instance)
		_instance = new QueryValidator;
	return _instance;

}

bool QueryValidator::isValidQuery(std::string str) {
	st = StringTokenizer(str, DelimiterMode::QUERY_PREPROCESSOR);
	/*
	if (!st.hasMoreTokens()) {
		return false;
	}
	*/
	return isSynonymDelcaration(st.nextToken());
}

bool QueryValidator::isSynonymDelcaration(std::string str) {
	bool isUnderProc = true;
	bool isValid = false;
	bool isSelectSyntax = isMatch(str, QueryValidator::SYNTAX_SELECT);
	bool isProcedureSyntax = isMatch(str, QueryValidator::SYNTAX_PROCEDURE);

	while (isUnderProc) {

		if (!st.hasMoreTokens()) {
			return false;
		}

		std::string nextToken = st.nextToken();



		if (isMatch(nextToken, ";")) { // end of procedure
			isUnderProc = false;
			return isValid;
		}

		if (isName(nextToken)) { // is a synonym
			isValid = true;
		}

		if (isMatch(nextToken, ",") && isName(st.peekNextToken())) { // ,[synonym] -> there's more synonym
			isValid = true;
		}

	}

	return isValid;

	/*
	if (isSelectSyntax || !isProcedureSyntax) return false;

	st.nextToken();

	while (isUnderProc) {

		std::string nextToken = st.nextToken();

		if (isMatch(nextToken, ";")) { // end of procedure
			isUnderProc = false;
			return isValid;
		}

		if (isName(nextToken)) { // is a procedure name
			isValid = true;
		}

		if (isMatch(nextToken, ",") && isName(st.peekNextToken())) {
			isValid = true;
		}

	}

	return isValid;
	*/
}

bool QueryValidator::isMatch(std::string s1, std::string s2) {
	return s1 == s2;
}

bool QueryValidator::isName(std::string str) {
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

