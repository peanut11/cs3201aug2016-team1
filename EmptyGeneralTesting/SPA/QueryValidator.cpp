#include <cctype>
#include <algorithm>
#include <string>
#include "QueryValidator.h"
#include "SynonymObject.h"
#include "ClauseType.h"

const std::string QueryValidator::SYNTAX_PROCEDURE = "procedure";
const std::string QueryValidator::SYNTAX_ASSIGN = "assign";
const std::string QueryValidator::SYNTAX_WHILE = "while";
const std::string QueryValidator::SYNTAX_IF = "if";
const std::string QueryValidator::SYNTAX_STATEMENT = "stmt";
const std::string QueryValidator::SYNTAX_VARIABLE = "variable";
const std::string QueryValidator::SYNTAX_CONSTANT = "constant";
const std::string QueryValidator::SYNTAX_PROG_LINE = "prog_line";
const std::string QueryValidator::SYNTAX_SELECT = "Select";
const std::string QueryValidator::SYNTAX_SUCH = "such";
const std::string QueryValidator::SYNTAX_THAT = "that";
const std::string QueryValidator::SYNTAX_SUCH_THAT = "such that";
const std::string QueryValidator::SYNTAX_WITH = "with";
const std::string QueryValidator::SYNTAX_AND = "and";
const std::string QueryValidator::SYNTAX_PATTERN = "pattern";

const std::string QueryValidator::SYNTAX_NEXT_LINE = "\n";
const std::string QueryValidator::SYNTAX_CALL = "call";

QueryValidator *QueryValidator::_instance;

/*
Singleton
*/
QueryValidator *QueryValidator::getInstance()
{
	if (!_instance)
		_instance = new QueryValidator;
	_instance->mSynonymTable = SynonymTable::getInstance();
	return _instance;
}


void QueryValidator::clearSynonymTable() {
	this->mSynonymTable->clearAll();
}

SynonymTable *QueryValidator::getSynonymTable() {
	return this->mSynonymTable;
}

bool QueryValidator::isValidQuery(std::string str) {
	st = StringTokenizer(str, DelimiterMode::QUERY_PREPROCESSOR);

	// validate both synonym declaration & select clause string
	return isDeclaration(st.nextToken()) && isSelect(st.hasMoreTokens() ? st.nextToken() : "");  //

}


bool QueryValidator::isDeclaration(std::string str) {

	EntityType mEntityType = getSyntaxEntityType(str);
	std::string entitySyntax = getEntitySyntax(str);

	SynonymObject mSynonymObject;

	bool isUnderDeclaration = true;
	bool isValid = false;

	while (isUnderDeclaration) {

		if (!st.hasMoreTokens()) {
			return false;
		}

		std::string nextToken = st.nextToken();

		if (isMatch(nextToken, ";")) { // end of specific synonym declaration

			if (!st.hasMoreTokens()) { // after ";" is empty
				return false;
			}

			if (isMatch(st.peekNextToken(), SYNTAX_NEXT_LINE)) { // after ";" is \n (end of declaration)
				st.nextToken();
				return isValid;
			}

			mEntityType = getSyntaxEntityType(st.peekNextToken()); // get type of entity (procedure, assign, if etc...)
			entitySyntax = getEntitySyntax(st.peekNextToken());

			st.nextToken();

			if (mEntityType != EntityType::INVALID && entitySyntax != "") {
				nextToken = st.nextToken();
				isUnderDeclaration = true;
				isValid = true;
			}
			else {
				isUnderDeclaration = false;
				isValid = false;
				break;
				//return isValid;
			}
		}

		if (isName(nextToken)) { // is a synonym
			isValid = true;
			isUnderDeclaration = true;

			mSynonymObject = SynonymObject(mEntityType, nextToken);
			bool insertStatus = this->mSynonymTable->insert(mSynonymObject);

			if (!insertStatus) { // failed to insert synonym due to duplicate copy
				return false;
			}
		}

		if (isMatch(nextToken, ",")) {
			isValid = isName(st.peekNextToken()); // ,[synonym] -> there's more synonym
		}

	}

	return isValid;

}

bool QueryValidator::isSelect(std::string str) {
	
	bool isUnderSelect = true;
	bool isValid = false;
	bool hasSelectOnce = false;
	ClauseType::ClauseType previousClauseType = ClauseType::SELECT;

	hasSelectOnce = isMatch(str, QueryValidator::SYNTAX_SELECT); // is the first word "Select"

	if (!hasSelectOnce) { return false; }

	while (isUnderSelect) {

		if (!st.hasMoreTokens()) { // select statement ends by no next token
			return isValid;
		}

		std::string nextToken = st.nextToken();

		if (isMatch(nextToken, QueryValidator::SYNTAX_SELECT) && hasSelectOnce) {
			// next string is "Select" again & already had "Select" previously
			// cannot have more than 1 "Select"
			return false;
		}

		if (isMatch(nextToken, QueryValidator::SYNTAX_SUCH) && isMatch(st.peekNextToken(), QueryValidator::SYNTAX_THAT)) {
			// first token = such & next token =  that
			previousClauseType = ClauseType::SUCH_THAT;
		}


		
		switch (previousClauseType) {

		case ClauseType::SELECT:
			isValid = isDeclaredSynonym(nextToken);
			break; 

		case ClauseType::SUCH_THAT:	

			break;
		}

	}

	return isValid;
	
}

bool QueryValidator::isMatch(std::string s1, std::string s2) {
	//std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	//std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	return s1 == s2;
}

bool QueryValidator::isDeclaredSynonym(std::string str) {
	return this->mSynonymTable->contains(str);
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


EntityType QueryValidator::getSyntaxEntityType(std::string syntax) {
	if (isMatch(syntax, SYNTAX_PROCEDURE)) {
		return EntityType::PROCEDURE;
	}
	else if (isMatch(syntax, SYNTAX_ASSIGN)) {
		return EntityType::ASSIGN;
	}
	else if (isMatch(syntax, SYNTAX_IF)) {
		return EntityType::IF;
	}
	else if (isMatch(syntax, SYNTAX_STATEMENT)) {
		return EntityType::STMT;
	}
	else if (isMatch(syntax, SYNTAX_VARIABLE)) {
		return EntityType::VARIABLE;
	}
	else if (isMatch(syntax, SYNTAX_CONSTANT)) {
		return EntityType::CONSTANT;
	}
	else if (isMatch(syntax, SYNTAX_PROG_LINE)) {
		return EntityType::PROGRAM_LINE;
	}
	else if (isMatch(syntax, SYNTAX_CALL)) {
		return EntityType::CALL;
	}
	else {
		return EntityType::INVALID;
	}


}

ClauseType::ClauseType QueryValidator::getClauseType(std::string syntax) {
	if (isMatch(syntax, SYNTAX_SELECT)) {
		return ClauseType::SELECT;
	}
	else if (isMatch(syntax, SYNTAX_SUCH_THAT)) {
		return ClauseType::SUCH_THAT;
	}
	else if (isMatch(syntax, SYNTAX_WITH)) {
		return ClauseType::WITH;
	}
	else if (isMatch(syntax, SYNTAX_AND)) {
		return ClauseType::AND;
	}
	else if (isMatch(syntax, SYNTAX_PATTERN)) {
		return ClauseType::PATTERN;
	}
	else {
		return ClauseType::INVALID;
	}
}


std::string QueryValidator::getEntitySyntax(std::string str) {
	if (isMatch(str, SYNTAX_PROCEDURE)) {
		return SYNTAX_PROCEDURE;
	}
	else if (isMatch(str, SYNTAX_ASSIGN)) {
		return SYNTAX_ASSIGN;
	}
	else if (isMatch(str, SYNTAX_IF)) {
		return SYNTAX_IF;
	}
	else if (isMatch(str, SYNTAX_STATEMENT)) {
		return SYNTAX_STATEMENT;
	}
	else if (isMatch(str, SYNTAX_VARIABLE)) {
		return SYNTAX_VARIABLE;
	}
	else if (isMatch(str, SYNTAX_CONSTANT)) {
		return SYNTAX_CONSTANT;
	}
	else if (isMatch(str, SYNTAX_PROG_LINE)) {
		return SYNTAX_PROG_LINE;
	}
	else if (isMatch(str, SYNTAX_CALL)) {
		return SYNTAX_CALL;
	}
	else {
		return "";
	}

}

std::string QueryValidator::getEntityTypeString(EntityType type) {
	switch (type) {
	case PROCEDURE:
		return "procedure";
	case ASSIGN:
		return "assign";
	case IF:
		return "if";
	}
}

std::string QueryValidator::getNextToken() {
	return st.nextToken();
	//return st.hasMoreTokens() ? st.nextToken() : "";
}


