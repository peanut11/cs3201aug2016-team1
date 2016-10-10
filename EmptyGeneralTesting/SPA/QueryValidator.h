#pragma once

#include <string>
#include <vector>

#include "AttrType.h"
#include "ClauseType.h"
#include "ClauseSuchThatArgObject.h"
#include "ClauseWithObject.h"
#include "ClauseWithRefObject.h"
#include "EntityType.h"
#include "QueryTable.h"
#include "RelationshipType.h"
#include "RelTable.h"
#include "StringTokenizer.h"
#include "SynonymTable.h"
#include "SynonymObject.h"
#include "SynonymOccurence.h"
#include "SynonymGroup.h"

// #include <cctype>
// #include <algorithm>
// #include "RelObject.h"
// #include "ClauseSelectObject.h"

class QueryValidator {

	~QueryValidator();

	static const std::string SYNTAX_PROCEDURE;
	static const std::string SYNTAX_ASSIGN;
	static const std::string SYNTAX_WHILE;
	static const std::string SYNTAX_IF;
	static const std::string SYNTAX_STATEMENT;
	static const std::string SYNTAX_VARIABLE;
	static const std::string SYNTAX_CONSTANT;
	static const std::string SYNTAX_PROG;
	static const std::string SYNTAX_LINE;
	static const std::string SYNTAX_PROG_LINE;
	static const std::string SYNTAX_CALL;

	static const std::string SYNTAX_SELECT;
	static const std::string SYNTAX_SUCH;
	static const std::string SYNTAX_THAT;
	static const std::string SYNTAX_SUCH_THAT;
	static const std::string SYNTAX_WITH;
	static const std::string SYNTAX_AND;
	static const std::string SYNTAX_PATTERN;
	static const std::string SYNTAX_NEXT_LINE;
	static const std::string SYNTAX_UNDERSCORE;
	static const std::string SYNTAX_DOUBLE_QUOTE;
	static const std::string SYNTAX_COMMA;
	static const std::string SYNTAX_EQUAL;
	static const std::string SYNTAX_BOOLEAN;
	static const std::string SYNTAX_STAR;
	static const std::string SYNTAX_DOT;
	static const std::string SYNTAX_LEFT_ARROW_BRACKET;
	static const std::string SYNTAX_RIGHT_ARROW_BRACKET;

	static const std::string SYNTAX_RELATIONSHIP_PARENT;
	static const std::string SYNTAX_RELATIONSHIP_PARENT_STAR;
	static const std::string SYNTAX_RELATIONSHIP_FOLLOWS;
	static const std::string SYNTAX_RELATIONSHIP_FOLLOWS_STAR;
	static const std::string SYNTAX_RELATIONSHIP_MODIFIES;
	static const std::string SYNTAX_RELATIONSHIP_USES;
	static const std::string SYNTAX_RELATIONSHIP_CALLS;
	static const std::string SYNTAX_RELATIONSHIP_CALLS_STAR;
	static const std::string SYNTAX_RELATIONSHIP_NEXT;
	static const std::string SYNTAX_RELATIONSHIP_NEXT_STAR;
	static const std::string SYNTAX_RELATIONSHIP_AFFECTS;
	static const std::string SYNTAX_RELATIONSHIP_AFFECTS_STAR;

	static const std::string SYNTAX_ATTRIBUTE_PROCEDURE_NAME;
	static const std::string SYNTAX_ATTRIBUTE_VARIABLE_NAME;
	static const std::string SYNTAX_ATTRIBUTE_STATEMENT;
	static const std::string SYNTAX_ATTRIBUTE_HEX;
	static const std::string SYNTAX_ATTRIBUTE_STATEMENT_NUMBER;
	static const std::string SYNTAX_ATTRIBUTE_VALUE;

	static QueryValidator *_instance;
	SynonymOccurence *mSynonymOccurence;
	SynonymTable *mSynonymTable;
	SynonymGroup *mSynonymGroup;
	RelTable *mRelTable;
	QueryTable mQueryTable;

	StringTokenizer st = StringTokenizer("", DelimiterMode::QUERY_PREPROCESSOR);

	ClauseSelectObject createSelectObject(EntityType entityType, AttrType::AttrType attrType, std::string synonymString, bool isBoolean);
	
	// Clause Such that object
	ClauseSuchThatObject* createClauseSuchThatObject(RelationshipType mRelType, ClauseSuchThatArgObject* firstArg, ClauseSuchThatArgObject* secondArg);
	ClauseSuchThatArgObject createClauseSuchThatArgObject(EntityType type, std::string stringValue, int integerValue, bool isSynonym);
	
	// Clause Pattern object
 	ClausePatternObject createClausePatternObject(EntityType patternType, EntityType firstArgType, bool isFirstArgSynonym, std::string patternSynonymArg, std::string firstArg, std::string secondArg);
	ClausePatternObject createClausePatternObject(EntityType patternType, EntityType firstArgType, bool isFirstArgSynonym, std::string firstArg, std::string patternSynonymArg, std::string secondArg, std::string thirdArg);

	
	// Clause With object
	ClauseWithObject createClauseWithObject(ClauseWithRefObject firstArg, ClauseWithRefObject secondArg);
	ClauseWithRefObject createClauseWithRefObject(WithRefType refType, std::string synonym, AttrType::AttrType attributeName);
	ClauseWithRefObject createClauseWithRefObject(WithRefType refType, std::string stringValue);
	ClauseWithRefObject createClauseWithRefObject(WithRefType refType, int integerValue);

	RelationshipType getSyntaxRelationshipType(std::string syntax);
	EntityType getSyntaxEntityType(std::string syntax);
	ClauseType::ClauseType getClauseType(std::string syntax);
	AttrType::AttrType getAttrType(std::string syntax);
	
	std::string getEntitySyntax(std::string str);
	std::string getRelationshipSyntax(RelationshipType type);
	std::string getAttrSyntax(AttrType::AttrType type);

	bool isMatch(std::string s1, std::string s2);
	//bool isSelect(std::string str);
	bool isInteger(std::string str);

	bool isFactor(std::string str);
	//bool isExpression(std::string str);
	bool isName(std::string str);
	bool isConstant(std::string str);
	bool isWildcard(std::string str);

	//bool isSynonym(std::string str);
	//bool isVariable(std::string str);
	//bool isArguments(std::string str);
	//bool isStatementNumber(std::string str);
	bool isNumber(std::string str);


	// Synonym declaration
	bool isDeclaration(std::string str); // procedure, assign ,if, stmt, variable, constant etc
	bool isDeclaredSynonym(std::string str); // pass in a string, check against synonymtable
	//bool isRelationship(std::string str); // string can be a relationship (parent, follows, etc)

	bool isSynonymContainsAttrName(EntityType type, std::string attrName);


	/*
	bool isStmt(std::string str);		// stmt
	bool isWhile(std::string str);		// while
	bool isIf(std::string str);			// if
	bool isVariable(std::string str);	// variable
	bool isConstant(std::string str);	// constant
	*/

	void insertSynonymGroup(ClauseSuchThatObject* object);
	void insertSynonymGroup(ClauseWithObject object);
	void insertSynonymGroup(ClausePatternObject object);


public:
	static QueryValidator *getInstance();

	static void deallocateMemory();

	void initStringTokenizer(std::string str);
	void clearSynonymOccurence();
	void clearSynonymTable();
	void clearSynonymGroup();
	void clearQueryTable();
	

	void addClauseSuchThatObject(std::vector<ClauseSuchThatObject*>& objects, ClauseSuchThatObject* object);

	// Return methods
	SynonymOccurence *getSynonymOccurence();
	SynonymTable *getSynonymTable();
	SynonymGroup *getSynonymGroup();
	RelTable *getRelationshipTable();
	QueryTable& getQueryTable();

	std::string validatedVariableName;
	std::string validatedExpression;

	std::string getEntityTypeString(EntityType type);
	std::string getNextToken();
	bool isSelect(std::string str);
	bool isValidQuery(std::string str);

	bool isVariable(std::string str); // contains double quote (e.g. "x")
	bool isVariableName(std::string str); // does not contain double quote (e.g. x)
	bool isRelationshipArgument(std::string str, RelObject relationshipObject);
	bool isRelationship(std::string str);
	bool isStatementNumber(std::string str);
	bool isSynonym(std::string str);
	bool isClauseResult(std::string str);
	bool isClauseWith(std::string str);
	bool isClausePattern(std::string str);
	bool isPatternExprArgument(std::string str);
	bool isExpression(std::string str);
	bool isTuple(std::string str);
	bool isAttributeReference(std::string str);
	bool isSyntaxBoolean(std::string str);

	int totalArg;
};