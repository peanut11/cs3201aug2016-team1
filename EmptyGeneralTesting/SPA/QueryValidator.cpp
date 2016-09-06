#include <cctype>
#include <algorithm>
#include <string>
#include "QueryValidator.h"
#include "SynonymObject.h"
#include "RelObject.h"
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
const std::string QueryValidator::SYNTAX_CALL = "call";

const std::string QueryValidator::SYNTAX_NEXT_LINE = "\n";
const std::string QueryValidator::SYNTAX_UNDERSCORE = "_";
const std::string QueryValidator::SYNTAX_DOUBLE_QUOTE = "\"";
const std::string QueryValidator::SYNTAX_COMMA = ",";


const std::string QueryValidator::SYNTAX_RELATIONSHIP_PARENT = "Parent";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_FOLLOWS = "Follows";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_MODIFIES = "Modifies";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_USES = "Uses";

const std::string QueryValidator::SYNTAX_ATTRIBUTE_PROCEDURE_NAME = "procName";
const std::string QueryValidator::SYNTAX_ATTRIBUTE_VARIABLE_NAME = "varName";
const std::string QueryValidator::SYNTAX_ATTRIBUTE_STATEMENT_NUMBER = "stmt#";
const std::string QueryValidator::SYNTAX_ATTRIBUTE_VALUE = "value";

QueryValidator *QueryValidator::_instance;

/*
Singleton
*/
QueryValidator *QueryValidator::getInstance()
{
	if (!_instance)
		_instance = new QueryValidator;
	_instance->mSynonymTable = SynonymTable::getInstance();
	_instance->mRelTable = RelTable::getInstance();
	return _instance;
}

void QueryValidator::initStringTokenizer(std::string str) {
	this->st = StringTokenizer(str, DelimiterMode::QUERY_PREPROCESSOR);
}

void QueryValidator::clearSynonymTable() {
	this->mSynonymTable->clearAll();
}

SynonymTable *QueryValidator::getSynonymTable() {
	return this->mSynonymTable;
}

RelTable *QueryValidator::getRelationshipTable() {
	return this->mRelTable;
}

bool QueryValidator::isValidQuery(std::string str) {
	bool hasDeclared = false;
	bool hasSelect = false;
	
	this->initStringTokenizer(str);

	// Declaration is a must for every query
	hasDeclared = isDeclaration(st.nextToken());
	if (!hasDeclared) { return false; }

	hasSelect = isSelect(st.hasMoreTokens() ? st.nextToken() : "");

	return hasSelect;
	// validate both synonym declaration & select clause string
	//return isDeclaration(st.nextToken()) && isSelect(st.hasMoreTokens() ? st.nextToken() : "");  //

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

		if (isSynonym(nextToken)) { // is a synonym
			isValid = true;
			isUnderDeclaration = true;

			mSynonymObject = SynonymObject(mEntityType, nextToken);
			bool insertStatus = this->mSynonymTable->insert(mSynonymObject);

			if (!insertStatus) { // failed to insert synonym due to duplicate copy
				return false;
			}
		}

		if (isMatch(nextToken, ",")) {
			isValid = isSynonym(st.peekNextToken()); // ,[synonym] -> there's more synonym
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

		if (isMatch(nextToken, QueryValidator::SYNTAX_SELECT)) {

			if (hasSelectOnce) {
				// next string is "Select" again & already had "Select" previously
				// cannot have more than 1 "Select"
				return false;
			}
		}

		// if current point to "such" clause
		if (isMatch(nextToken, QueryValidator::SYNTAX_SUCH) && isMatch(st.peekNextToken(), QueryValidator::SYNTAX_THAT)) {
			// first token = "such" & next token = "that"
			previousClauseType = ClauseType::SUCH_THAT;
			st.nextToken(); // point to "that" clause

			//isValid = isDeclaredSynonym(nextToken);
		}

		if (isDeclaredSynonym(nextToken)) {

			switch (previousClauseType) {

			case ClauseType::SELECT:
				isValid = true;
				break;

			case ClauseType::SUCH_THAT:

				break;
			}

		}

		if (isRelationship(nextToken)) {

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

bool QueryValidator::isClauseWith(std::string str) {
	return false;
}

bool QueryValidator::isClausePattern(std::string str) {
	SynonymObject selectedSynonymObj;
	bool isUnderPattern = true;
	int numOfComma = 0;
	int maxNumOfArgs = 0;
	int numOfArgs = 0;
	
	// start with pattern syntax
	if (str.compare(SYNTAX_PATTERN) != 0) { return false; }

	st.nextToken(); // point to pattern syntax

	// after pattern syntax, a synonym (assign, while, if) must be written next
	if (!isSynonym(st.peekNextToken())) { return false; }

	selectedSynonymObj = this->mSynonymTable->getObject(st.peekNextToken());
	if (selectedSynonymObj.getType() == EntityType::INVALID) {
		return true; // invalid synonym, did not declare in the first place
	}

	if (selectedSynonymObj.getType() != EntityType::ASSIGN
		&& selectedSynonymObj.getType() != EntityType::WHILE
		&& selectedSynonymObj.getType() != EntityType::IF) {
		return false; // since the synonym is not within one of these, its wrong
	}

	if (selectedSynonymObj.getType() == EntityType::ASSIGN || selectedSynonymObj.getType() == EntityType::WHILE) {
		maxNumOfArgs = 2;
	}
	else {
		maxNumOfArgs = 3; // entity type if
	}

	st.nextToken(); // point to valid synonym

	// check if there's open bracket
	if (st.peekNextToken().compare("(") != 0) { return false;  }
	st.nextToken(); // point to "("

	while (isUnderPattern) {

		if (!st.hasMoreTokens()) { return false; }

		if (st.peekNextToken().compare(")") == 0) { // end of pattern
			currentNumberOfArgument = numOfArgs;
			if (numOfArgs == maxNumOfArgs) {
				return true;
			}
			else {
				return false;
			}
		}

		std::string nextToken = st.nextToken();

		if (isMatch(nextToken, SYNTAX_COMMA)) {
			numOfComma += 1;
		}

		// first argument, if synonym, assign must match with assign, same for while and if
		if (numOfArgs == 0 && numOfComma == 0) {
			if (isSynonym(nextToken)) {

				// WHILE and IF cannot have synonym as control variable
				if (selectedSynonymObj.getType() == ASSIGN 
					&& this->mSynonymTable->getObject(nextToken).getType() == selectedSynonymObj.getType()) {
						numOfArgs += 1;
				}
				
			}
			else if (isVariable(nextToken)) {
				// first arugment, if it is a variable name
				// WHILE and IF can have variable as control variable
				numOfArgs += 1;
			}
			else if (isWildcard(nextToken)) {
				// first arugment, if it is a "_" (wildcard)
				// WHILE and IF can have wildcard as control variable
				numOfArgs += 1;
			}
		}

		else if (numOfArgs >= 1 && numOfArgs <= maxNumOfArgs && numOfComma > 0) {
			// check current number of args; second arugment, validate expression

			if (!isMatch(nextToken, SYNTAX_COMMA)) {
				if (isWildcard(nextToken)) { // "_"
					numOfArgs += 1;
				}
				else if (isPatternExprArgument(nextToken)) { // "x + y"
					// isMatch(nextToken, "\"") // start double quote
					// && isMatch(nextToken, "\"") // end double quote

					// "x + y" or _"x+y"_

					// Only WHILE cannot have pattern behind
					if (selectedSynonymObj.getType() != EntityType::WHILE) {
						numOfArgs += 1;
					}
					
				}
			}

			


		}

	}

}

/*
@str	current token which is the relationship syntax 
*/
bool QueryValidator::isRelationship(std::string str) {

	RelationshipType searchedType = getSyntaxRelationshipType(str);

	if (searchedType == RelationshipType::INVALID_RELATIONSHIP) {
		// no such relationship
		return false;
	}

	// Relationship(args1,args2) e.g. Parent(ifstmt, w)

	RelObject searchedRelObject = this->mRelTable->find(searchedType);
	if (searchedRelObject.getRelObjectType() == RelationshipType::INVALID_RELATIONSHIP) {
		// no such relationship in table
		return false;
	}

	st.nextToken(); // point to relationship
	
	return isArguments(st.peekNextToken(), searchedRelObject); 
	// isMatch(st.nextToken(), "(")
	// && isMatch(st.nextToken(), ")")
		
	//&& isMatch(st.nextToken(), ")")
	// check is synonym or variable
	// synonym has no double quote, check with synonym table
	// variable has double quote, may include underscore, validate variable
	



}

bool QueryValidator::isArguments(std::string str, RelObject relationshipObject) {

	bool isUnderArg = true;
	bool isValid = false;
	bool hasComma = false;
	bool hasValidFirstArg = false;

	int numberOfArgs = 0;

	if (str == "" || str.compare("(") != 0) {
		return false;
	}

	st.nextToken(); // point to "("

	while (isUnderArg) {

		if (!st.hasMoreTokens()) { return false; }

		if (isMatch(st.peekNextToken(), ")")) {	// 2 arguments have finished and next token is a close bracket
			if (numberOfArgs == relationshipObject.getNumOfArgs()) {
				return true;
			}
			else {
				return false;
			}
		}

		std::string nextToken = st.nextToken();

		if (isStatementNumber(nextToken)) {	// integer of statement# (e.g. 1, 2)

			// havent read first argument and current token is correct first arugment type
			if (!hasValidFirstArg) {
				if (relationshipObject.doesFirstArgsContains(EntityType::STMT)) {
					hasValidFirstArg = true; // first argument is correct
					numberOfArgs += 1;
				}
				else {
					//isValid = false;
				}
				
			}

			// has read first arugment and comma and current token is correct second arugment type
			if (hasComma && hasValidFirstArg) {

				if (relationshipObject.doesSecondArgsContains(EntityType::STMT)) {
					//isValid = true;
					numberOfArgs += 1;
				}
				else {
					//isValid = false;
				}
				
			}

		}
		
		if (isSynonym(nextToken)) {
			
			// check SynonymTable to get EntityType, and check if the EntityType is valid for the relationship
			if (!hasValidFirstArg) {

				if (relationshipObject.doesFirstArgsContains(this->mSynonymTable->getObject(nextToken).getType())) {
					hasValidFirstArg = true; // first argument is correct
					numberOfArgs += 1;
				}
				else {
					// means this relationship does not contain the specific arg type
					//isValid = false;
				}
	
			}

			// has read first arugment and comma and current token is correct second arugment type
			if (hasComma && hasValidFirstArg) {

				if (relationshipObject.doesSecondArgsContains(this->mSynonymTable->getObject(nextToken).getType())) {
					//isValid = true;
					numberOfArgs += 1;
				}
				else {
					//isValid = false;
				}

			}
		}
		

		if (isMatch(nextToken, SYNTAX_COMMA) && hasValidFirstArg) {
			hasComma = true;
		}

	}

	return false;

}

bool QueryValidator::isPatternExprArgument(std::string str) {
	
	bool isUnderArg = true;
	bool containWildcard = false;
	bool isValidExpression = false;
	int numofDoubleQuote = 0;
	int numOfWildcard = 0;

	if (str.compare("") == 0) { // empty
		return false;
	}

	if (str.compare(SYNTAX_UNDERSCORE) == 0) {
		containWildcard = true;
		numOfWildcard += 1;
	}
	else if (str.compare("\"") == 0) {
		//isValidExpression = isExpression(st.nextToken());
		numofDoubleQuote += 1;
	}


	while (isUnderArg) {
		if (!st.hasMoreTokens()) { return false; }

		if ((isMatch(st.peekNextToken(), SYNTAX_COMMA) || isMatch(st.peekNextToken(), ")"))) {
			if (containWildcard && numOfWildcard == 2 && numofDoubleQuote == 2) {
				return true;
			}
			else if (numofDoubleQuote == 1 || numOfWildcard == 1) {
				return false; // did not end a second quote and jump to next comma or close bracket
			}
			else if (numofDoubleQuote == 2 && numOfWildcard == 0) {

				return isValidExpression; // only have double quotes
			}
			else {
				return false;
			}
		}
		
		std::string nextToken = st.nextToken();

		if (nextToken.compare(SYNTAX_UNDERSCORE) == 0) {
			//if (numOfWildcard == 0) { // first wildcard
			//	containWildcard = true;
			//}
			numOfWildcard += 1;
		}
		else if (nextToken.compare("\"") == 0) {
			numofDoubleQuote += 1;
		}

		if (numofDoubleQuote == 1) { // passed first quote
			if (nextToken.compare("\"") != 0) { // don't want compare the same double quote
				isValidExpression = isExpression(nextToken);

				if (!isValidExpression) {
					return false;
				}
			}
			
		}




		/*
		if (nextToken.compare("\"") == 0) {
			if (numofDoubleQuote == 0) {
				isValidExpression = isExpression(st.nextToken());	
			}
			numofDoubleQuote += 1;
		}
		*/
	}

	




}

bool QueryValidator::isSynonym(std::string str) {
	return isName(str);
}

bool QueryValidator::isVariable(std::string str) {
	if (isMatch(str, SYNTAX_UNDERSCORE)) {
		return false;
	}

	if (!isMatch(str, SYNTAX_DOUBLE_QUOTE)) {
		return false;
	}

	//st.nextToken();

	return (st.hasMoreTokens() ? isName(st.nextToken()) : false)
		&& (st.hasMoreTokens() ? isMatch(st.nextToken(), SYNTAX_DOUBLE_QUOTE) : false);

}

bool QueryValidator::isVariableName(std::string str) {
	return isName(str);
}

bool QueryValidator::isExpression(std::string str) {
	if (isFactor(str)) {
		if (isMatch(st.peekNextToken(), "+")) {
			st.popNextToken();
			return isExpression(st.nextToken());
		}
		return true;
	}
	return false;
}

bool QueryValidator::isFactor(std::string str) {
	return isVariableName(str) || isConstant(str);
}

bool QueryValidator::isConstant(std::string str) {
	return isInteger(str);
}

bool QueryValidator::isWildcard(std::string str) {
	return (str.compare(SYNTAX_UNDERSCORE) == 0);
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

bool QueryValidator::isStatementNumber(std::string str) {
	if (str.empty()) {
		return false;
	}

	if (std::isdigit(str[0])) {
		return true;
	}

	return false;
}

bool QueryValidator::isInteger(std::string str) {
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

RelationshipType QueryValidator::getSyntaxRelationshipType(std::string syntax) {
	if (isMatch(syntax, SYNTAX_RELATIONSHIP_USES)) {
		return RelationshipType::USES;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_MODIFIES)) {
		return RelationshipType::MODIFIES;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_PARENT)) {
		return RelationshipType::PARENT;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_FOLLOWS)) {
		return RelationshipType::FOLLOWS;
	}
	else {
		return RelationshipType::INVALID_RELATIONSHIP;
	}
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
	else if (isMatch(syntax, SYNTAX_WHILE)) {
		return EntityType::WHILE;
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
	else if (isMatch(str, SYNTAX_WHILE)) {
		return SYNTAX_WHILE;
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
	case WHILE:
		return "while";
	}

	return "";
}

std::string QueryValidator::getNextToken() {
	//return st.nextToken();
	return st.hasMoreTokens() ? st.nextToken() : "";
}


