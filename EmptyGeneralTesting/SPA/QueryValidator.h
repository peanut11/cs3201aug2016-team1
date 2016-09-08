#pragma once
#include <string>
#include <vector>
#include "StringTokenizer.h"
#include "EntityType.h"
#include "RelationshipType.h"
#include "ClauseType.h"
#include "AttrType.h"
#include "SynonymTable.h"
#include "SynonymObject.h"
#include "SynonymOccurence.h"
#include "RelTable.h"
#include "PatternObject.h"
#include "WithObject.h"
#include "WithRefObject.h"
#include "WithAttrObject.h"
#include "QueryTable.h"

class QueryValidator {

	static const std::string SYNTAX_PROCEDURE;
	static const std::string SYNTAX_ASSIGN;
	static const std::string SYNTAX_WHILE;
	static const std::string SYNTAX_IF;
	static const std::string SYNTAX_STATEMENT;
	static const std::string SYNTAX_VARIABLE;
	static const std::string SYNTAX_CONSTANT;
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
	static const std::string SYNTAX_BOOLEAN;

	static const std::string SYNTAX_RELATIONSHIP_PARENT;
	static const std::string SYNTAX_RELATIONSHIP_FOLLOWS;
	static const std::string SYNTAX_RELATIONSHIP_MODIFIES;
	static const std::string SYNTAX_RELATIONSHIP_USES;

	static const std::string SYNTAX_ATTRIBUTE_PROCEDURE_NAME;
	static const std::string SYNTAX_ATTRIBUTE_VARIABLE_NAME;
	static const std::string SYNTAX_ATTRIBUTE_STATEMENT_NUMBER;
	static const std::string SYNTAX_ATTRIBUTE_VALUE;

	static QueryValidator *_instance;
	SynonymOccurence *mSynonymOccurence;
	SynonymTable *mSynonymTable;
	RelTable *mRelTable;

	QueryTable *mQueryTable;


	StringTokenizer st = StringTokenizer("", DelimiterMode::QUERY_PREPROCESSOR);

	QueryTable *getQueryTable();

	SelectObject createClauseSelectObject(EntityType entityType, AttrType::AttrType attrType, bool isBoolean);
	//SuchThatObject createClauseSuchThatRelObject(RelationshipType mRelType, SuchThatArgObject firstArg, SuchThatArgObject secondArg);
	SuchThatArgObject createClauseSuchThatArgObject(EntityType type, bool isSynonym, std::string stringValue, int integerValue);
	//PatternObject createClausePatternObject();
	WithObject createClauseWithObject();

	RelationshipType getSyntaxRelationshipType(std::string syntax);
	EntityType getSyntaxEntityType(std::string syntax);
	ClauseType::ClauseType getClauseType(std::string syntax);
	
	std::string getEntitySyntax(std::string str);


	bool isMatch(std::string s1, std::string s2);
	//bool isSelect(std::string str);
	bool isInteger(std::string str);

	bool isBoolean(std::string str); // BOOLEAN
	bool isSuchThat(std::string str); // match such+that
	bool isAnd(std::string str);
	bool isFollow(std::string str);
	bool isParent(std::string str);
	bool isPattern(std::string str); // match pattern+assign+(+"variable"|_+,+"exp"|_+)
	bool isWith(std::string str);

	bool isFactor(std::string str);
	//bool isExpression(std::string str);
	bool isName(std::string str);
	bool isConstant(std::string str);
	bool isWildcard(std::string str);

	//bool isSynonym(std::string str);
	//bool isVariable(std::string str);
	//bool isArguments(std::string str);
	//bool isStatementNumber(std::string str);

	// Synonym declaration
	bool isDeclaration(std::string str); // procedure, assign ,if, stmt, variable, constant etc
	bool isDeclaredSynonym(std::string str); // pass in a string, check against synonymtable
	//bool isRelationship(std::string str); // string can be a relationship (parent, follows, etc)


	/*
	bool isStmt(std::string str);		// stmt
	bool isWhile(std::string str);		// while
	bool isIf(std::string str);			// if
	bool isVariable(std::string str);	// variable
	bool isConstant(std::string str);	// constant
	*/


public:
	static QueryValidator *getInstance();

	void initStringTokenizer(std::string str);
	void clearSynonymOccurence();
	void clearSynonymTable();

	// Return methods
	SynonymOccurence *getSynonymOccurence();
	SynonymTable *getSynonymTable();
	RelTable *getRelationshipTable();

	std::string getEntityTypeString(EntityType type);
	std::string getNextToken();
	bool isSelect(std::string str);
	bool isValidQuery(std::string str);

	bool isVariable(std::string str); // contains double quote (e.g. "x")
	bool isVariableName(std::string str); // does not contain double quote (e.g. x)
	bool isArguments(std::string str, RelObject relationshipObject);
	bool isRelationship(std::string str);
	bool isStatementNumber(std::string str);
	bool isSynonym(std::string str);
	bool isClauseResult(std::string str);
	bool isClauseWith(std::string str);
	bool isClausePattern(std::string str);
	bool isPatternExprArgument(std::string str);
	bool isExpression(std::string str);
	bool isTurple(std::string str);
	bool isSyntaxBoolean(std::string str);
};