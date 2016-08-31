#include <cctype>
#include <algorithm>
#include <string>
#include "QueryValidator.h"
#include "SynonymObject.h"

const std::string QueryValidator::SYNTAX_PROCEDURE = "procedure";
const std::string QueryValidator::SYNTAX_ASSIGN = "assign";
const std::string QueryValidator::SYNTAX_SELECT = "select";
const std::string QueryValidator::SYNTAX_NEXT_LINE = "\n";

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

	return isDeclareEntity(st.nextToken());

}


bool QueryValidator::isDeclareEntity(std::string str) {

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

			if (isMatch(nextToken, SYNTAX_NEXT_LINE)) { // after ";" is \n (end of declaration
				return isValid;
			}

			mEntityType = getSyntaxEntityType(st.peekNextToken());
			entitySyntax = getEntitySyntax(st.peekNextToken());

			st.nextToken();

			if (mEntityType != EntityType::INVALID && entitySyntax != "") {
				nextToken = st.nextToken();
				isUnderDeclaration = true;
				isValid = true;
			}
			else {
				isUnderDeclaration = false;
				return isValid;
			}
		}

		if (isName(nextToken)) { // is a synonym
			isValid = true;
			isUnderDeclaration = true;

			mSynonymObject = SynonymObject(mEntityType, nextToken);
			this->mSynonymTable->insert(mSynonymObject);
			//this->mSynonymTable.push_back(nextToken);
		}

		if (isMatch(nextToken, ",")) {
			isValid = isName(st.peekNextToken()); // ,[synonym] -> there's more synonym
		}

	}

	return isValid;

}


bool QueryValidator::isSelect(std::string str) {
	return isMatch(str, QueryValidator::SYNTAX_SELECT)
		&& isName(st.nextToken());

}

bool QueryValidator::isMatch(std::string s1, std::string s2) {
	std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
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


EntityType QueryValidator::getSyntaxEntityType(std::string syntax) {
	if (isMatch(syntax, SYNTAX_PROCEDURE)) {
		return EntityType::PROCEDURE;
	}
	else if (isMatch(syntax, SYNTAX_ASSIGN)) {
		return EntityType::ASSIGN;
	}
	else {
		return EntityType::INVALID;
	}
}

std::string QueryValidator::getEntitySyntax(std::string str) {
	if (isMatch(str, SYNTAX_PROCEDURE)) {
		return SYNTAX_PROCEDURE;
	}
	else if (isMatch(str, SYNTAX_ASSIGN)) {
		return SYNTAX_ASSIGN;
	}
	else {
		return "";
	}
}

std::string QueryValidator::getEntityTypeString(EntityType type) {
	switch (type) {
	case PROCEDURE:
		return "Procedure";
	case ASSIGN:
		return "Assign";
	}
}


