#pragma once
#include <string>
#include <vector>
#include "StringTokenizer.h"
#include "EntityType.h"
#include "ClauseType.h"
#include "SynonymTable.h"
#include "SynonymObject.h"

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

	static QueryValidator *_instance;
	SynonymTable *mSynonymTable;

	StringTokenizer st = StringTokenizer("", DelimiterMode::QUERY_PREPROCESSOR);

	EntityType getSyntaxEntityType(std::string syntax);
	ClauseType::ClauseType getClauseType(std::string syntax);
	std::string getEntitySyntax(std::string str);

	

	bool isMatch(std::string s1, std::string s2);
	//bool isSelect(std::string str);
	bool isBoolean(std::string str); // BOOLEAN
	bool isSuchThat(std::string str); // match such+that
	bool isAnd(std::string str);
	bool isFollow(std::string str);
	bool isParent(std::string str);
	bool isPattern(std::string str); // match pattern+assign+(+"variable"|_+,+"exp"|_+)
	bool isWith(std::string str);

	bool isExpression(std::string str);

	// Synonym declaration
	bool isDeclaration(std::string str); // procedure, assign ,if, stmt, variable, constant etc
	//bool isDeclaredSynonym(std::string str); // pass in a string, check against synonymtable

	/*
	bool isStmt(std::string str);		// stmt
	bool isWhile(std::string str);		// while
	bool isIf(std::string str);			// if
	bool isVariable(std::string str);	// variable
	bool isConstant(std::string str);	// constant
	*/
	bool isName(std::string str);		

	



	/*
	stmt s, s1; assign a, a1, a2; while w, w1; if ifstmt, iftstmt1; procedure p; variable v; constant c;
prog_line n, n1, n2;
	*/

public:
	static QueryValidator *getInstance();

	void clearSynonymTable();

	// Return methods
	SynonymTable *getSynonymTable();
	std::string getEntityTypeString(EntityType type);
	std::string getNextToken();
	bool isSelect(std::string str);
	bool isValidQuery(std::string str);
	bool isDeclaredSynonym(std::string str); // pass in a string, check against synonymtable
	

};