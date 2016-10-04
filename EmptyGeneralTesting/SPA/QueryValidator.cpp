#include <cctype>
#include <algorithm>
#include <string>
#include "QueryValidator.h"
#include "SynonymObject.h"
#include "RelObject.h"
#include "ClauseType.h"
#include "AttrType.h"
#include "SelectObject.h"
#include "ClauseSuchThatArgObject.h"


const std::string QueryValidator::SYNTAX_PROCEDURE = "procedure";
const std::string QueryValidator::SYNTAX_ASSIGN = "assign";
const std::string QueryValidator::SYNTAX_WHILE = "while";
const std::string QueryValidator::SYNTAX_IF = "if";
const std::string QueryValidator::SYNTAX_STATEMENT = "stmt";
const std::string QueryValidator::SYNTAX_VARIABLE = "variable";
const std::string QueryValidator::SYNTAX_CONSTANT = "constant";
const std::string QueryValidator::SYNTAX_PROG = "prog";
const std::string QueryValidator::SYNTAX_LINE = "line";
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
const std::string QueryValidator::SYNTAX_EQUAL = "=";
const std::string QueryValidator::SYNTAX_BOOLEAN = "BOOLEAN";
const std::string QueryValidator::SYNTAX_STAR = "*";
const std::string QueryValidator::SYNTAX_DOT = ".";
const std::string QueryValidator::SYNTAX_LEFT_ARROW_BRACKET = "<";
const std::string QueryValidator::SYNTAX_RIGHT_ARROW_BRACKET = ">";

const std::string QueryValidator::SYNTAX_RELATIONSHIP_PARENT = "Parent";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_PARENT_STAR = "Parent*";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_FOLLOWS = "Follows";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_FOLLOWS_STAR = "Follows*";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_MODIFIES = "Modifies";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_USES = "Uses";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_CALLS = "Calls";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_CALLS_STAR = "Calls*";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_NEXT = "Next";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_NEXT_STAR = "Next*";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_AFFECTS = "Affects";
const std::string QueryValidator::SYNTAX_RELATIONSHIP_AFFECTS_STAR = "Affects*";

const std::string QueryValidator::SYNTAX_ATTRIBUTE_PROCEDURE_NAME = "procName";
const std::string QueryValidator::SYNTAX_ATTRIBUTE_VARIABLE_NAME = "varName";
const std::string QueryValidator::SYNTAX_ATTRIBUTE_STATEMENT = "stmt";
const std::string QueryValidator::SYNTAX_ATTRIBUTE_HEX = "#";
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
	_instance->mSynonymOccurence = SynonymOccurence::getInstance();
	_instance->mSynonymTable = SynonymTable::getInstance();
	_instance->mRelTable = RelTable::getInstance();
	_instance->mQueryTable = QueryTable();

	return _instance;
}

void QueryValidator::deallocateMemory() {
	delete _instance;
}

/*
Destructor
*/
QueryValidator::~QueryValidator() {
	delete _instance->mSynonymOccurence;
	delete _instance->mSynonymTable;
	delete _instance->mRelTable;
}


void QueryValidator::initStringTokenizer(std::string str) {
	this->st = StringTokenizer(str, DelimiterMode::QUERY_PREPROCESSOR);
}

void QueryValidator::clearSynonymOccurence() {
	this->mSynonymOccurence->clearAll();
}

void QueryValidator::clearSynonymTable() {
	this->mSynonymTable->clearAll();
}

void QueryValidator::clearQueryTable() {
	this->mQueryTable.clearAll();
}

void QueryValidator::addClauseSuchThatObject(std::vector<ClauseSuchThatObject>& objects, ClauseSuchThatObject object) {
	objects.push_back(object);
}

void QueryValidator::throwsInvalidExpression(std::string expression) {
	throw std::runtime_error("Invalid expression found near " + expression);
}

void QueryValidator::throwsInvalidRelationship(RelationshipType type) {
	throw std::runtime_error("Invalid query near relationship " + getRelationshipSyntax(type));
}

void QueryValidator::throwsInvalidRelationshipArgument(RelationshipType type, std::string arugment) {
	throw std::runtime_error("Invalid argument for relationship " + getRelationshipSyntax(type) + " near " + arugment);
}

void QueryValidator::throwsInvalidRelationshipSameSynonymArguments(RelationshipType type, std::string argument1, std::string argument2) {
	throw std::runtime_error("Invalid relationship " + getRelationshipSyntax(type) + " due to same synonym for both argument arg1 = " + argument1 + " and arg2 = " + argument2);
}

void QueryValidator::throwsIncorrectSyntax(std::string syntax) {
	throw std::runtime_error("Invalid syntax near " + syntax);
}

void QueryValidator::throwsInvalidPattern(std::string syntax) {
	throw std::runtime_error("Invalid pattern near " + syntax);
}

void QueryValidator::throwsInvalidPatternMissingSyntax() {
	throw std::runtime_error("Invalid pattern, missing syntax");
}

void QueryValidator::throwsInvalidPatternTypeSyntax() {
	throw std::runtime_error("Invalid pattern syntax type");
}

void QueryValidator::throwsInvalidPatternArgument() {
	throw std::runtime_error("Invalid pattern argument(s)");
}

void QueryValidator::throwsInvalidPatternArgument(std::string arugment) {
	throw std::runtime_error("Invalid pattern argument near " + arugment);
}

void QueryValidator::throwsInvalidPatternExpression(std::string expression) {
	throw std::runtime_error("Invalid pattern expression near " + expression);
}

void QueryValidator::throwsInvalidAttributeFormat() {
	throw std::runtime_error("Invalid attribute format");
}

void QueryValidator::throwsInvalidAttributeMissingSynonym() {
	throw std::runtime_error("Invalid attribute missing synonym");
}

void QueryValidator::throwsInvalidAttributeSyntax(std::string syntax) {
	throw std::runtime_error("Invalid attribute syntax near " + syntax);
}

void QueryValidator::throwsInvalidAttributeName(std::string syntax) {
	throw std::runtime_error("Invalid attribute name near " + syntax);
}

void QueryValidator::throwsInvalidAttributeSingleSynonym(std::string syntax) {
	throw std::runtime_error("Invalid attribute single synonym near " + syntax);
}

void QueryValidator::throwsInvalidAttributeValue(std::string value) {
	throw std::runtime_error("Invalid attribute value near" + value);
}

void QueryValidator::throwsExceedCommonSynonymCount() {
	throw std::runtime_error("Exceed number of common synonym between clauses");
}

QueryTable& QueryValidator::getQueryTable() {
	return this->mQueryTable;
}

SelectObject QueryValidator::createSelectObject(EntityType entityType, AttrType::AttrType attrType, std::string synonymString, bool isBoolean) {
	return SelectObject(entityType, synonymString, attrType, isBoolean);
}

// Clause Such that object
ClauseSuchThatObject QueryValidator::createClauseSuchThatObject(RelationshipType mRelType, ClauseSuchThatArgObject firstArg, ClauseSuchThatArgObject secondArg) {
	return ClauseSuchThatObject(mRelType, firstArg, secondArg);
}

ClauseSuchThatArgObject QueryValidator::createClauseSuchThatArgObject(EntityType type, std::string stringValue, int integerValue, bool isSynonym) {
	return ClauseSuchThatArgObject(type, stringValue, integerValue, isSynonym);
}

// Clause Pattern object

ClausePatternObject QueryValidator::createClausePatternObject(EntityType patternType, EntityType firstArgType, bool isFirstArgSynonym, std::string patternSynonymArg, std::string firstArg, std::string secondArg) {
	return ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg);
}

ClausePatternObject QueryValidator::createClausePatternObject(EntityType patternType, EntityType firstArgType, bool isFirstArgSynonym, std::string patternSynonymArg, std::string firstArg, std::string secondArg, std::string thirdArg) {
	return ClausePatternObject(patternType, patternSynonymArg, firstArgType, isFirstArgSynonym, firstArg, secondArg, thirdArg);
}



// Clause With object
ClauseWithObject QueryValidator::createClauseWithObject(ClauseWithRefObject firstArg, ClauseWithRefObject secondArg) {
	return ClauseWithObject(firstArg, secondArg);
}

ClauseWithRefObject QueryValidator::createClauseWithRefObject(WithRefType refType, std::string synonym, AttrType::AttrType attributeName) {
	return ClauseWithRefObject(refType, synonym, attributeName);
}

ClauseWithRefObject QueryValidator::createClauseWithRefObject(WithRefType refType, std::string stringValue) {
	return ClauseWithRefObject(refType, stringValue);
}

ClauseWithRefObject QueryValidator::createClauseWithRefObject(WithRefType refType, int integerValue) {
	return ClauseWithRefObject(refType, integerValue);
}

SynonymOccurence *QueryValidator::getSynonymOccurence() {
	return this->mSynonymOccurence;
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
	this->clearQueryTable();
	this->clearSynonymOccurence();
	this->clearSynonymTable();

	this->validatedVariableName = "";
	this->validatedExpression = "";

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
			else if (isMatch(st.peekNextToken(), SYNTAX_SELECT)) {
				return isValid;
			}


			if (isMatch(st.peekNextToken(), SYNTAX_PROG)) { // start is prog
				st.nextToken();
				if (isMatch(st.nextToken(), SYNTAX_UNDERSCORE) && isMatch(st.nextToken(), SYNTAX_LINE)) {
					mEntityType = EntityType::PROGRAM_LINE;
					entitySyntax = SYNTAX_PROG_LINE;
				}
			}
			else {
				mEntityType = getSyntaxEntityType(st.peekNextToken()); // get type of entity (procedure, assign, if etc...)
				entitySyntax = getEntitySyntax(st.peekNextToken());
				st.nextToken();
			}

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
	bool isValid = true;
	bool hasSelectOnce = false;
	ClauseType::ClauseType previousClauseType = ClauseType::SELECT;

	hasSelectOnce = isMatch(str, QueryValidator::SYNTAX_SELECT); // is the first word "Select"

	if (!hasSelectOnce) { return false; }

	while (isUnderSelect) {

		if (!st.hasMoreTokens()) { // select statement ends by no next token
			return isValid;
		}

		if (!isValid) { 
			return false;
		}

		std::string currentToken = st.nextToken();

		if (isMatch(currentToken, QueryValidator::SYNTAX_SELECT)) {
			previousClauseType = ClauseType::SELECT;
			if (hasSelectOnce) {
				// next string is "Select" again & already had "Select" previously
				// cannot have more than 1 "Select"
				return false;
			}
		}

		// if current point to "such" clause
		else if (isMatch(currentToken, QueryValidator::SYNTAX_SUCH) && isMatch(st.nextToken(), QueryValidator::SYNTAX_THAT)) {
			// first token = "such" & next token = "that"
			previousClauseType = ClauseType::SUCH_THAT;
		}
		 
		else if (isMatch(currentToken, QueryValidator::SYNTAX_PATTERN)) {
			previousClauseType = ClauseType::PATTERN;
		}
		else if (isMatch(currentToken, QueryValidator::SYNTAX_AND)) {
			// remain the same previous clause type
		}
		else if (isMatch(currentToken, QueryValidator::SYNTAX_WITH)) {
			previousClauseType = ClauseType::WITH;
		}
		else {
			// not clauses (such that, with, pattern, and)
			switch (previousClauseType) {

			case ClauseType::SELECT:
				isValid = isClauseResult(currentToken);

				if (isValid) {
					
					// insert into clause select table
					EntityType mSynonymEntityType = this->mSynonymTable->getObject(currentToken).getType();

					if (isSyntaxBoolean(currentToken)) {  // BOOLEAN
						this->getQueryTable().replaceSelectObject(SelectObject(EntityType::INVALID, "", AttrType::INVALID, true));
					}
					else if (isSynonym(currentToken)	// synonym
						&& mSynonymEntityType != EntityType::INVALID) {

						this->getQueryTable().replaceSelectObject(SelectObject(mSynonymEntityType, currentToken, AttrType::INVALID, false));
					}

					// insert into synonym occurence table
					//this->mSynonymOccurence->setIncrementOccurence(currentToken, ClauseType::SELECT);

				}

				break;

			case ClauseType::SUCH_THAT:
				isValid = isRelationship(currentToken);
				break;

			case ClauseType::WITH:
				isValid = isClauseWith(currentToken);
				break;

			case ClauseType::PATTERN:
				isValid = isClausePattern(currentToken);
				break;
			}
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

bool QueryValidator::isClauseResult(std::string str) {
	if (isTuple(str)) {
		return true;
	}
	else if (isSyntaxBoolean(str)) {
		return true;
	}
	return false;
}

bool QueryValidator::isClauseWith(std::string str) {
	
	AttrType::AttrType mAttributeType = AttrType::AttrType::INVALID;
	SynonymObject selectedSynonymObj;
	ClauseWithRefObject leftRefObject;
	ClauseWithRefObject rightRefObject;
		

	std::string currentToken;
	
	bool hasValidAttrValue = false;	// check if value after "=" exists	
	bool hasValidSynonym = false;	// check if synonym declared is valid
	bool hasValidProgLine = false;	// check if prog_line is correctly used e.g. pl = 2
	bool hasDot = false;			// check if "." exists
	bool hasAttribute = false;		// check if attribute (procName, varName) exists
	bool hasEqualSign = false;

	bool isUnderWith = true;
	bool isValueInteger = false;	// true if integer, false if string
	bool flag = false;

	bool hasFirstAttribute = false;

	/*
	1. Any statement synonyms have attr = stmt# e.g. w.stmt# = 2
	2. Procedure synonym has attr = procName e.g. p.procName = "test"
	3. Call synonym has attr = procName e.g. c.procName = "test" and c.stmt# = 4
	4. Variable synonym has attr = varName e.g. var.varName = "hello"
	5. Constant synonym has attr = value
	*/


	while (isUnderWith) {
		
		
		if (hasValidProgLine || (hasAttribute && hasValidAttrValue)) {
			this->mQueryTable.insertWithObject(ClauseWithObject(leftRefObject, rightRefObject));
			return true;
		}
		

		if (!flag) {
			currentToken = str;
			flag = true;
		}
		else {
			currentToken = st.nextToken();
		}

		if (isMatch(currentToken, this->SYNTAX_EQUAL)) {
			hasEqualSign = true;
		}


		
		if (!hasValidSynonym && this->isSynonym(currentToken) && this->isDeclaredSynonym(currentToken)) {

			selectedSynonymObj = this->mSynonymTable->getObject(str); // get synonym object which is found in synonymTable
			if (selectedSynonymObj.getType() == EntityType::INVALID) { // check declared
				this->throwsInvalidPattern(str);
				isUnderWith = false;
			}

			hasValidSynonym = true;

			if (selectedSynonymObj.getType() == EntityType::PROGRAM_LINE && isMatch(st.peekNextToken(), this->SYNTAX_EQUAL)) {
				leftRefObject = this->createClauseWithRefObject(WithRefType::SYNONYM, selectedSynonymObj.getSynonym(), AttrType::INVALID);
			}



		} // end of synonym

		if (hasValidSynonym) {
			if (isMatch(currentToken, this->SYNTAX_DOT)) {
				hasDot = true;
				continue; // goto next while cycle
			}
		}

		if (isMatch(currentToken, this->SYNTAX_EQUAL)) {
			hasEqualSign = true;
			continue; // goto next while cycle
		}

		
		if (hasValidSynonym && hasDot && !hasAttribute) {

			switch (selectedSynonymObj.getType()) {
			case STMT:
			case IF:
			case WHILE:
			case ASSIGN:
				if (isMatch(currentToken, this->SYNTAX_ATTRIBUTE_STATEMENT) && isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
					mAttributeType = AttrType::AttrType::STMT_NO;
					hasAttribute = true;
					st.nextToken(); // points to "#"
				}
				else {
					this->throwsInvalidAttributeName(currentToken);
					isUnderWith = false;
				}
				break;

			case PROCEDURE:
				if (isMatch(currentToken, this->SYNTAX_ATTRIBUTE_PROCEDURE_NAME)) {
					mAttributeType = AttrType::AttrType::PROC_NAME;
					hasAttribute = true;
				}
				else {
					this->throwsInvalidAttributeName(currentToken);
					isUnderWith = false;
				}
				break;

			case CALL:
				if (isMatch(currentToken, this->SYNTAX_ATTRIBUTE_STATEMENT) && isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
					mAttributeType = AttrType::AttrType::STMT_NO;
					hasAttribute = true;
					st.nextToken(); // points to "#"
				}
				else if (isMatch(currentToken, this->SYNTAX_ATTRIBUTE_PROCEDURE_NAME)) {
					mAttributeType = AttrType::AttrType::PROC_NAME;
					hasAttribute = true;
				}
				else {
					this->throwsInvalidAttributeName(currentToken);
					isUnderWith = false;
				}
				break;

			case VARIABLE:
				if (isMatch(currentToken, this->SYNTAX_ATTRIBUTE_VARIABLE_NAME)) {
					mAttributeType = AttrType::AttrType::VAR_NAME;
					hasAttribute = true;
				}
				else {
					this->throwsInvalidAttributeName(currentToken);
					isUnderWith = false;
				}
				break;

			case CONSTANT:
				if (isMatch(currentToken, this->SYNTAX_ATTRIBUTE_VALUE)) {
					mAttributeType = AttrType::AttrType::VALUE;
					hasAttribute = true;
				}
				else {
					this->throwsInvalidAttributeName(currentToken);
					isUnderWith = false;
				}
				break;

			}

			if (hasAttribute) {
				leftRefObject = this->createClauseWithRefObject(WithRefType::ATTRREF, 
					selectedSynonymObj.getSynonym(), 
					mAttributeType);
			}


		}
		else if (hasValidSynonym && hasDot && hasAttribute) {

			if (hasEqualSign && !isMatch(currentToken,this->SYNTAX_EQUAL)) { // has EQUAL but current token is not EQUAL

				switch (mAttributeType) {
				case AttrType::AttrType::STMT_NO:
				case AttrType::AttrType::VALUE:
					// check if value is integer
					if (isNumber(currentToken)) {

						hasValidAttrValue = true;
						// declare right attr reference object
						rightRefObject = this->createClauseWithRefObject(WithRefType::INTEGER, atoi(currentToken.c_str()));

					}
					else if (isSynonym(currentToken) && isDeclaredSynonym(currentToken) && isMatch(st.peekNextToken(), this->SYNTAX_DOT)) { // another synonym attribute on the right

						AttrType::AttrType type = AttrType::INVALID;
						std::string secondSynonym = currentToken; // save the synonym value

						currentToken = st.nextToken(); // point to "."

						if (isMatch(st.peekNextToken(), SYNTAX_STATEMENT)) {
							currentToken = st.nextToken(); // point to stmt
							if (isMatch(st.peekNextToken(), SYNTAX_ATTRIBUTE_HEX)) {
								currentToken = st.nextToken(); // point to #

								type = AttrType::STMT_NO;
							}
						}
						else {
							type = this->getAttrType(st.peekNextToken());
						}

						if (type == AttrType::VALUE || type == AttrType::STMT_NO) {
							st.nextToken(); // point to second attrName
							hasValidAttrValue = true;

							// declare right attr reference object
							rightRefObject = this->createClauseWithRefObject(WithRefType::ATTRREF, 
																				secondSynonym, 
																				type);

						}

					}
					else {
						this->throwsInvalidAttributeValue(currentToken);
						isUnderWith = false;
					}
					break;

				case AttrType::AttrType::PROC_NAME:
				case AttrType::AttrType::VAR_NAME:
					// check if value is string with double quotes
					if (isVariable(currentToken)) {

						hasValidAttrValue = true;
						// declare right attr reference object
						rightRefObject = this->createClauseWithRefObject(WithRefType::IDENTIFIER, this->validatedVariableName);

					}
					else if (isSynonym(currentToken) && isDeclaredSynonym(currentToken) && isMatch(st.peekNextToken(), this->SYNTAX_DOT)) { // another synonym attribute on the right
						
						st.nextToken(); // point to "."

						AttrType::AttrType type = this->getAttrType(st.peekNextToken());
						std::string secondSynonym = currentToken; // save the synonym value

						if (type == AttrType::PROC_NAME || type == AttrType::VAR_NAME) {
							st.nextToken(); // point to second attrName
							hasValidAttrValue = true;

							// declare right attr reference object
							rightRefObject = this->createClauseWithRefObject(WithRefType::ATTRREF,
								secondSynonym,
								type);
						}

					}
					else {
						this->throwsInvalidAttributeValue(currentToken);
						isUnderWith = false;
					}
					break;
				}

				
			}

		}
		else if (hasValidSynonym && !hasDot && !hasAttribute) { // there exists synonym but without dot and attribute 
			
			if (hasEqualSign && !isMatch(currentToken, this->SYNTAX_EQUAL)) { // has EQUAL but current token is not EQUAL
				if (isNumber(currentToken)) {
					hasValidAttrValue = true;
					hasValidProgLine = true;
					// declare right attr reference object
					rightRefObject = this->createClauseWithRefObject(WithRefType::INTEGER, atoi(currentToken.c_str()));
				}
				else { // Programe Line must have integer value after "=" 
					this->throwsInvalidAttributeSingleSynonym(selectedSynonymObj.getSynonym());
					isUnderWith = false;
				}
			}

		}
		else {
			this->throwsInvalidAttributeFormat();
			isUnderWith = false;
		}

		
	}

	return false;
}

bool QueryValidator::isClausePattern(std::string str) {
	SynonymObject selectedSynonymObj;

	EntityType firstArgType;
	std::string firstArg, secondArg, thirdArg;
	bool isFirstArgSynonym = false;
	bool isUnderPattern = true;
	int numOfComma = 0;
	int maxNumOfArgs = 0;
	int numOfArgs = 0;
	
	// start is a synonym (assign, while, if) must be written next
	if (!isSynonym(str)) { 
		this->throwsInvalidPatternMissingSyntax();
		//return false; 
	}

	selectedSynonymObj = this->mSynonymTable->getObject(str); // st.peekNextToken()
	if (selectedSynonymObj.getType() == EntityType::INVALID) {
		this->throwsInvalidPattern(str);
		//return false; // invalid synonym, did not declare in the first place
	}

	if (selectedSynonymObj.getType() != EntityType::ASSIGN
		&& selectedSynonymObj.getType() != EntityType::WHILE
		&& selectedSynonymObj.getType() != EntityType::IF) {

		this->throwsInvalidPatternTypeSyntax();
		return false; // since the synonym is not within one of these, its wrong
	}

	if (selectedSynonymObj.getType() == EntityType::ASSIGN || selectedSynonymObj.getType() == EntityType::WHILE) {
		maxNumOfArgs = 2;
	}
	else {
		maxNumOfArgs = 3; // entity type if
	}

	// check if reach max synonym occurence
	//if (this->getSynonymOccurence()->hasMaximumOccurence(str, ClauseType::PATTERN)) { return false; }
	// update synonym occurence
	this->getSynonymOccurence()->setIncrementOccurence(str, ClauseType::PATTERN);

	//st.nextToken(); // point to valid synonym

	// check if there's open bracket
	if (st.peekNextToken().compare("(") != 0) {  
		this->throwsInvalidPattern(st.peekNextToken());
		//return false;  
	}
	st.nextToken(); // point to "("

	while (isUnderPattern) {

		if (!st.hasMoreTokens()) { return false; }

		if (st.peekNextToken().compare(")") == 0) { // end of pattern
			st.nextToken(); // point to ")"
			
			this->totalArg = numOfArgs;
			
			if (numOfArgs == maxNumOfArgs) { // WHILE and ASSIGN have max 2 args, IF has max 3 args

				// check the number of common synonyms in clauses
				if (this->mSynonymOccurence->hasMaxCommonSynonym()) {
					this->throwsExceedCommonSynonymCount();
				}

				if (numOfArgs == 2) {
					this->mQueryTable.insertPatternObject(
						this->createClausePatternObject(selectedSynonymObj.getType(), firstArgType, isFirstArgSynonym, str, firstArg, secondArg));
				}
				else { // third argument 
					this->mQueryTable.insertPatternObject(
						this->createClausePatternObject(selectedSynonymObj.getType(), firstArgType, isFirstArgSynonym, str, firstArg, secondArg, thirdArg));
				}

				return true;
			}
			else {
				this->throwsInvalidPatternArgument();
				//return false;
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
					&& this->mSynonymTable->getObject(nextToken).getType() == EntityType::VARIABLE) { //selectedSynonymObj.getType()
					
					firstArgType = EntityType::VARIABLE;
					isFirstArgSynonym = true;
					firstArg = nextToken;
					numOfArgs += 1;
				}
				
			}
			else if (isVariable(nextToken)) {
				// first arugment, if it is a variable
				// WHILE and IF can have variable as control variable

				firstArgType = EntityType::VARIABLE;
				isFirstArgSynonym = false;
				firstArg = this->validatedVariableName; // validated variable name is correct
				numOfArgs += 1;
			}
			else if (isWildcard(nextToken)) {
				// first arugment, if it is a "_" (wildcard)
				// WHILE and IF can have wildcard as control variable
				firstArgType = EntityType::WILDCARD;
				isFirstArgSynonym = false;
				firstArg = nextToken;
				numOfArgs += 1;
			}
		}

		else if (numOfArgs == 1 && numOfComma == 1) { // && numOfArgs <= maxNumOfArgs 
			// check current number of args; second arugment, validate expression

			if (!isMatch(nextToken, SYNTAX_COMMA)) {
				if (isWildcard(nextToken) && !isMatch(st.peekNextToken(), SYNTAX_DOUBLE_QUOTE)) { // "_" without double quote for next token
					secondArg = nextToken;
					numOfArgs += 1;
				}
				else if (isPatternExprArgument(nextToken)) { // "x + y"

					// Only WHILE cannot have pattern behind
					if (selectedSynonymObj.getType() != EntityType::WHILE) {
						secondArg = this->validatedExpression;
						numOfArgs += 1;
					}
					
				}
			} 
			
		}
		else if (numOfArgs == 2 && numOfComma == 2) {
			// third argument which is for IF
			if (!isMatch(nextToken, SYNTAX_COMMA) && selectedSynonymObj.getType() == EntityType::IF) {
				if ((isWildcard(nextToken) 
					&& !isMatch(st.peekNextToken(), SYNTAX_DOUBLE_QUOTE))
					|| isPatternExprArgument(nextToken)) { // "_" or "x + y"

					thirdArg = validatedExpression;
					numOfArgs += 1;
				}
				
			}

		}

	}

	return false;
}

/*
@str	current token which is the relationship syntax 
*/
bool QueryValidator::isRelationship(std::string str) {

	if (isMatch(st.peekNextToken(), SYNTAX_STAR)) { // next is a star!
		str += st.nextToken(); // e.g. Follows*, Parent*, Next*, Affects*, Calls*
	}

	RelationshipType searchedType = getSyntaxRelationshipType(str);

	if (searchedType == RelationshipType::INVALID_RELATIONSHIP) {
		// no such relationships
		this->throwsInvalidRelationship(searchedType);
		//return false;
	}

	// Relationship(args1,args2) e.g. Parent(stmt1, stmt2)
	
	RelObject searchedRelObject = this->mRelTable->find(searchedType);
	if (searchedRelObject.getRelObjectType() == RelationshipType::INVALID_RELATIONSHIP) {
		// no such relationship in table
		this->throwsInvalidRelationship(searchedRelObject.getRelObjectType());
		//return false;
	}

	//st.nextToken(); // point to relationship
	
	return isRelationshipArgument(st.peekNextToken(), searchedRelObject);

}

bool QueryValidator::isRelationshipArgument(std::string str, RelObject relationshipObject) {

	ClauseSuchThatArgObject firstArgObject;
	ClauseSuchThatArgObject secondArgObject;

	bool isUnderArg = true;
	bool hasComma = false;
	bool hasValidFirstArg = false;

	int numberOfArgs = 0;

	if (str == "" || str.compare("(") != 0) {
		this->throwsInvalidRelationshipArgument(relationshipObject.getRelObjectType(), str);
		//return false;
	}

	st.nextToken(); // point to "("

	while (isUnderArg) {

		if (!st.hasMoreTokens()) { return false; }

		if (isMatch(st.peekNextToken(), ")")) {	// 2 arguments have finished and next token is a close bracket
			st.nextToken(); // point to ")"

			if (numberOfArgs == relationshipObject.getNumOfArgs()) {
				
				// check the number of common synonyms in clauses
				//if (this->mSynonymOccurence->hasMaxCommonSynonym()) {  
				//	this->throwsExceedCommonSynonymCount();
				//}

				if (relationshipObject.getRelObjectType() != RelationshipType::NEXT 
						&& relationshipObject.getRelObjectType() != RelationshipType::NEXT_STAR 
						&& firstArgObject.getIsSynonym() 
						&& secondArgObject.getIsSynonym() 
						&& isMatch(firstArgObject.getStringValue(), secondArgObject.getStringValue())) {
					this->throwsInvalidRelationshipSameSynonymArguments(relationshipObject.getRelObjectType(), firstArgObject.getStringValue(), secondArgObject.getStringValue());
					isUnderArg = false;
				}


				this->addClauseSuchThatObject(this->getQueryTable().getSuchThats(),
					this->createClauseSuchThatObject(relationshipObject.getRelObjectType(), firstArgObject, secondArgObject));

				return true;
			}
			else {
				this->throwsInvalidRelationshipArgument(relationshipObject.getRelObjectType(), st.peekNextToken());
				isUnderArg = false;
			}
		}

		std::string nextToken = st.nextToken();

		if (isStatementNumber(nextToken)) {	// integer of statement# (e.g. 1, 2)

			// havent read first argument and current token is correct first arugment type
			if (!hasValidFirstArg) {

				if (relationshipObject.doesFirstArgsContains(EntityType::CONSTANT)) {

					firstArgObject = ClauseSuchThatArgObject(EntityType::CONSTANT,
															"", atoi(nextToken.c_str()), false);

					hasValidFirstArg = true; // first argument is correct
					numberOfArgs += 1;
				}				
			}

			// has read first arugment and comma and current token is correct second arugment type
			if (hasComma && hasValidFirstArg) {

				if (relationshipObject.doesSecondArgsContains(EntityType::CONSTANT)) {
					secondArgObject = ClauseSuchThatArgObject(EntityType::CONSTANT, "", 
						atoi(nextToken.c_str()), false);
					numberOfArgs += 1;
				}
				
			}

		}
		
		else if (isSynonym(nextToken)) {
			
			// check SynonymTable to get EntityType, and check if the EntityType is valid for the relationship
			if (!hasValidFirstArg) {

				if (relationshipObject.doesFirstArgsContains(this->mSynonymTable->getObject(nextToken).getType())) {
					
					// update synonym occurence
					//this->getSynonymOccurence()->setIncrementOccurence(nextToken, ClauseType::SUCH_THAT);
						
					firstArgObject = ClauseSuchThatArgObject(this->mSynonymTable->getObject(nextToken).getType(),
						nextToken, ClauseSuchThatArgObject::EMTPY_INT, true);

					hasValidFirstArg = true; // first argument is correct
					numberOfArgs += 1;
					
				}

			}

			// has read first arugment and comma and current token is correct second arugment type
			if (hasComma && hasValidFirstArg) {

				if (relationshipObject.doesSecondArgsContains(this->mSynonymTable->getObject(nextToken).getType())) {

					// update synonym occurence
					//this->getSynonymOccurence()->setIncrementOccurence(nextToken, ClauseType::SUCH_THAT);
						
					secondArgObject = ClauseSuchThatArgObject(this->mSynonymTable->getObject(nextToken).getType(),
						nextToken, ClauseSuchThatArgObject::EMTPY_INT, true);

					numberOfArgs += 1;
				}

			}
		} // end of isSynonym

		else if (isVariable(nextToken)) {
			
			// variable only used for MODIFIES and USES
			// and always a second argument
			if ((relationshipObject.getRelObjectType() == RelationshipType::MODIFIES
				|| relationshipObject.getRelObjectType() == RelationshipType::USES) 
				&& hasComma && hasValidFirstArg) {

				secondArgObject = ClauseSuchThatArgObject(EntityType::VARIABLE, this->validatedVariableName,
					0, false);

				hasValidFirstArg = true; // first argument is correct
				numberOfArgs += 1;
			}

		}

		else if (isWildcard(nextToken)) {
			// first argument is wildcard

			if (!hasValidFirstArg) {
				// if modifies and uses
				// cannot have wildcard as first argument

				if (!(relationshipObject.getRelObjectType() == RelationshipType::MODIFIES 
					|| relationshipObject.getRelObjectType() == RelationshipType::USES)) {

					firstArgObject = ClauseSuchThatArgObject(EntityType::WILDCARD,
						nextToken, ClauseSuchThatArgObject::EMTPY_INT, false);

					hasValidFirstArg = true; // first argument is correct
					numberOfArgs += 1;
				}
				
			}

			// second argument is wildcard
			if (hasComma && hasValidFirstArg) {

				secondArgObject = ClauseSuchThatArgObject(EntityType::WILDCARD,
					nextToken, ClauseSuchThatArgObject::EMTPY_INT, false);

				numberOfArgs += 1;
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
		this->throwsInvalidPatternArgument();
		//return false;
	}

	if (str.compare(SYNTAX_UNDERSCORE) == 0) {
		this->validatedExpression.append("_");
		containWildcard = true;
		numOfWildcard += 1;
	}
	else if (str.compare("\"") == 0) {
		//this->validatedExpression.append("\"");
		numofDoubleQuote += 1;
	}


	while (isUnderArg) {
		if (!st.hasMoreTokens()) { return false; }

		if ((isMatch(st.peekNextToken(), SYNTAX_COMMA) || isMatch(st.peekNextToken(), ")"))) {
			if (containWildcard && numOfWildcard == 2 && numofDoubleQuote == 2) {	
				return true;
			}
			else if (numofDoubleQuote == 1 || numOfWildcard == 1) {
				this->throwsInvalidPatternArgument(st.peekNextToken());
				//return false; // did not end a second quote and jump to next comma or close bracket
			}
			else if (numofDoubleQuote == 2 && numOfWildcard == 0) {

				return isValidExpression; // only have double quotes
			}
			else {
				this->throwsInvalidPatternArgument(st.peekNextToken());
				//return false;
			}
		}
		
		std::string nextToken = st.nextToken();

		if (nextToken.compare(SYNTAX_UNDERSCORE) == 0) {
			this->validatedExpression.append("_");
			numOfWildcard += 1;
		}
		else if (nextToken.compare("\"") == 0) {
			//this->validatedExpression.append("\"");
			numofDoubleQuote += 1;
		}

		if (numofDoubleQuote == 1) { // passed first quote
			if (nextToken.compare("\"") != 0) { // don't want compare the same double quote
				
				isValidExpression = isExpression(nextToken);

				// check valid for second argument
				if (!isValidExpression) {
					this->throwsInvalidPatternExpression(nextToken);
					//return false;
				}

				//this->validatedExpression.append(nextToken);
			}
			
		}

	}
	
	return false;
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

	this->validatedVariableName = "";
	//this->validatedVariableName.append(SYNTAX_DOUBLE_QUOTE);

	if (!isName(st.peekNextToken())) {
		return false;
	}

	this->validatedVariableName.append(st.nextToken());

	if (!isMatch(st.nextToken(), SYNTAX_DOUBLE_QUOTE)) {
		return false;
	}

	//this->validatedVariableName.append(SYNTAX_DOUBLE_QUOTE);

	return true;

}

bool QueryValidator::isVariableName(std::string str) {
	return isName(str);
}

bool QueryValidator::isExpression(std::string str) {
	this->validatedExpression.append(str);
	
	if (isFactor(str)) {
		if (isMatch(st.peekNextToken(), "+")) {
			this->validatedExpression.append(st.peekNextToken());
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

bool QueryValidator::isTuple(std::string str) {

	if (!isMatch(str, this->SYNTAX_LEFT_ARROW_BRACKET)) {
		if (isSynonym(str)
			&& this->isDeclaredSynonym(str)) {
			
			if (isMatch(st.peekNextToken(), this->SYNTAX_DOT)) {
				st.popNextToken();
				if (isSynonymContainsAttrName(this->getSynonymTable()->getObject(str).getType(), st.nextToken())) {
					return true;
				}
				else {
					return false;
				}
			}
		
			return true;
		}

		return false;
	}

	std::string currentToken = "";
	std::string previousValidSynonym = "";
	bool isWithinTuple = true;
	bool isValid = true;
	bool hasRightArrowBracket = false;
	
	int synonymCount = 0;


	while (isWithinTuple) {
		
		if (hasRightArrowBracket) {
			return isValid;
		}

		currentToken = st.nextToken();
		
		if (isMatch(currentToken, this->SYNTAX_RIGHT_ARROW_BRACKET)) {
			hasRightArrowBracket = true;
			continue;
		} 
		else if (isMatch(currentToken, this->SYNTAX_COMMA)) {
			if (isSynonym(st.peekNextToken()) 
				&& this->isDeclaredSynonym(st.peekNextToken())) {

				previousValidSynonym = st.nextToken(); // points to synonym, after ","
				isValid = true;
				synonymCount++;
			}
			else {
				isWithinTuple = false;
			}

			continue;
		}
		else if (isMatch(currentToken, this->SYNTAX_DOT)) {
			// means before that there's a synonym
			if (isSynonym(previousValidSynonym)
				&& this->isDeclaredSynonym(previousValidSynonym)
				&& this->isSynonymContainsAttrName(this->mSynonymTable->getObject(previousValidSynonym).getType(), st.nextToken())) {

				isValid = true;
				/*
				if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_STATEMENT)) {
					st.popNextToken();
					if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
						st.popNextToken();
						isValid = true;
					}
					else {
						isValid = false;
					}
				}
				else {
					st.popNextToken();
					isValid = true;
				}
				*/

			} else {
				isValid = false;
			}
		}
		// current token is a synonym with a "," or ">" 
		else if (isSynonym(currentToken) 
				&& (isMatch(st.peekNextToken(), this->SYNTAX_COMMA) 
					|| isMatch(st.peekNextToken(), this->SYNTAX_RIGHT_ARROW_BRACKET))) {

			if (this->isDeclaredSynonym(currentToken)) {
				previousValidSynonym = currentToken;
				isValid = true;
				synonymCount++;
			}
			else {
				isWithinTuple = false;
			}
			continue;
		}
		// current token is a synonym with a dot next
		else if (isSynonym(currentToken)
			&& isMatch(st.peekNextToken(), this->SYNTAX_DOT)) {

			st.popNextToken(); // remove "."

			if (this->isDeclaredSynonym(currentToken)
				&& isSynonymContainsAttrName(this->mSynonymTable->getObject(currentToken).getType(), st.nextToken())) {
				
				previousValidSynonym = currentToken;
				isValid = true;
				synonymCount++;

				/*
				if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_STATEMENT)) {
					st.popNextToken();
					if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
						st.popNextToken();

						previousValidSynonym = currentToken;
						isValid = true;
						synonymCount++;
					}
					else {
						isValid = false;
					}
				}
				else {
					st.popNextToken();
					previousValidSynonym = currentToken;
					isValid = true;
					synonymCount++;
				}
				*/

			}
			else {
				isWithinTuple = false;
				isValid = false;
			}

			continue;
		}
		else {
			isWithinTuple = false;
		}

	}

	return false;
}

bool QueryValidator::isSyntaxBoolean(std::string str) {
	return isMatch(str, SYNTAX_BOOLEAN);
}

bool QueryValidator::isAttributeReference(std::string str) {

	SynonymObject selectedSynonymObj;

	if (this->isSynonym(str) && this->isDeclaredSynonym(str)) {

		selectedSynonymObj = this->mSynonymTable->getObject(str); // get synonym object which is found in synonymTable
		if (selectedSynonymObj.getType() == EntityType::INVALID) { // check declared
			//this->throwsInvalidPattern(str);
			return false;
		}

		// prog_line does not need ".", no attrName
		if (selectedSynonymObj.getType() == EntityType::PROGRAM_LINE && !isMatch(st.peekNextToken(), this->SYNTAX_DOT)) {
			return true;
		}

		st.popNextToken(); // remove "." from stack

		return this->isSynonymContainsAttrName(selectedSynonymObj.getType(), st.nextToken());

		/*
		switch (selectedSynonymObj.getType()) {
		case STMT:
		case IF:
		case WHILE:
		case ASSIGN:
			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_STATEMENT)) {

				st.nextToken(); // points to stmt
				if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
					st.nextToken(); // points to "#"
					return true;
				}
				
			}
			else {
				return false;
			}
			break;

		case PROCEDURE:
			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_PROCEDURE_NAME)) {
				st.nextToken(); // points to procName
				return true;
			}
			else {
				return false;
			}
			break;

		case CALL:
			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_STATEMENT)) {

				st.nextToken(); // points to stmt
				if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
					st.nextToken(); // points to "#"
					return true;
				}

			}
			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_PROCEDURE_NAME)) {
				st.nextToken(); // points to procName
				return true;
			}
			else {
				return false;
			}
			break;

		case VARIABLE:
			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_VARIABLE_NAME)) {
				return true;
			}
			else {
				return false;
			}
			break;

		case CONSTANT:
			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_VALUE)) {
				return true;
			}
			else {
				return false;
			}
			break;
		}
		*/

	}

	return false;
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

	if (std::isdigit(str[0])) { //[0] atoi(str.c_str())
		if (atoi(str.c_str()) > 0) {
			return true;
		}
		return false;
	}

	return false;
}

bool QueryValidator::isNumber(std::string str) {
	if (str.empty()) {
		return false;
	}

	if (std::isdigit(str[0])) { //[0] atoi(str.c_str())
		if (atoi(str.c_str()) > 0) {
			return true;
		}
		return false;
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

bool  QueryValidator::isSynonymContainsAttrName(EntityType type, std::string attrName) {
	switch (type) {
	case STMT:
	case IF:
	case WHILE:
	case ASSIGN:
		if (isMatch(attrName, this->SYNTAX_ATTRIBUTE_STATEMENT)) {
			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
				st.popNextToken(); // remove "#" from stack
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
		break;

	case PROCEDURE:
		return isMatch(attrName, this->SYNTAX_ATTRIBUTE_PROCEDURE_NAME);
		break;

	case CALL:
		if (isMatch(attrName, this->SYNTAX_ATTRIBUTE_STATEMENT)) {

			if (isMatch(st.peekNextToken(), this->SYNTAX_ATTRIBUTE_HEX)) {
				st.popNextToken(); // remove "#" from stack
				return true;
			}
			else {
				return false;
			}

		} 
		else if (isMatch(attrName, this->SYNTAX_ATTRIBUTE_PROCEDURE_NAME)) {
			return true;
		}
		else {
			return false;
		}
		break;

	case VARIABLE:
		return isMatch(attrName, this->SYNTAX_ATTRIBUTE_VARIABLE_NAME);
		break;

	case CONSTANT:
		return isMatch(attrName, this->SYNTAX_ATTRIBUTE_VALUE);
		break;
	default:
		return false;
	}
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
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_PARENT_STAR)) {
		return RelationshipType::PARENT_STAR;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_FOLLOWS)) {
		return RelationshipType::FOLLOWS;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_FOLLOWS_STAR)) {
		return RelationshipType::FOLLOWS_STAR;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_CALLS)) {
		return RelationshipType::CALLS;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_CALLS_STAR)) {
		return RelationshipType::CALLSSTAR;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_NEXT)) {
		return RelationshipType::NEXT;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_NEXT_STAR)) {
		return RelationshipType::NEXT_STAR;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_AFFECTS)) {
		return RelationshipType::AFFECTS;
	}
	else if (isMatch(syntax, SYNTAX_RELATIONSHIP_AFFECTS_STAR)) {
		return RelationshipType::AFFECTS_STAR;
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

AttrType::AttrType QueryValidator::getAttrType(std::string syntax) {
	if (isMatch(syntax, SYNTAX_ATTRIBUTE_VALUE)) {
		return AttrType::AttrType::VALUE;
	} else if (isMatch(syntax, SYNTAX_ATTRIBUTE_STATEMENT_NUMBER)) {
		return AttrType::AttrType::STMT_NO;
	} else if (isMatch(syntax, SYNTAX_ATTRIBUTE_PROCEDURE_NAME)) {
		return AttrType::AttrType::PROC_NAME;
	} else if (isMatch(syntax, SYNTAX_ATTRIBUTE_VARIABLE_NAME)) {
		return AttrType::AttrType::VAR_NAME;
	}

	return AttrType::AttrType::INVALID;
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

std::string QueryValidator::getRelationshipSyntax(RelationshipType type) {
	switch (type) {
	case MODIFIES:
		return "modifies";
	case USES:
		return "uses";
	case CALLS:
		return "calls";
	case CALLSSTAR:
		return "calls*";
	case FOLLOWS:
		return "follows";
	case FOLLOWS_STAR:
		return "follows*";
	case PARENT_STAR:
		return "parent*";
	case NEXT:
		return "next";
	case NEXT_STAR:
		return "next*";
	case AFFECTS:
		return "affects";
	case AFFECTS_STAR:
		return "affects*";
	default:
		return "";
	}
}

std::string QueryValidator::getAttrSyntax(AttrType::AttrType type) {
	switch (type) {
	case AttrType::PROC_NAME:
		return SYNTAX_ATTRIBUTE_PROCEDURE_NAME;

	case AttrType::STMT_NO:
		return SYNTAX_ATTRIBUTE_STATEMENT_NUMBER;

	case AttrType::VAR_NAME:
		return SYNTAX_ATTRIBUTE_VARIABLE_NAME;

	case AttrType::VALUE:
		return SYNTAX_ATTRIBUTE_VALUE;
	}
}

std::string QueryValidator::getNextToken() {
	//return st.nextToken();
	return st.hasMoreTokens() ? st.nextToken() : "";
}


