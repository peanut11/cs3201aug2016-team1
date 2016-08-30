#pragma once
#include <string>
#include "StringTokenizer.h"

class QueryValidator {

	static const std::string SYNTAX_PROCEDURE;
	static const std::string SYNTAX_SELECT;

	static QueryValidator *_instance;

	StringTokenizer st = StringTokenizer("");
	//bool isValidSyntax();
	bool isMatch(std::string s1, std::string s2);
	

	
	bool isSelect(std::string str);
	bool isBoolean(std::string str); // BOOLEAN
	bool isSuchThat(std::string str); // match such+that
	bool isAnd(std::string str);
	bool isFollow(std::string str);
	bool isParent(std::string str);
	bool isPattern(std::string str); // match pattern+assign+(+"variable"|_+,+"exp"|_+)
	bool isWith(std::string str);

	bool isExpression(std::string str);
	bool isProcedure(std::string str);
	bool isName(std::string str);

public:
	static QueryValidator *getInstance();

	bool isSynonymDelcaration(std::string str);
	bool isEndOfSynonymDeclaration(std::string str); // end

	/*
	
	bool isInteger(std::string str);
	
	bool isStmtLst(std::string str);
	bool isStmt(std::string str);
	bool isWhile(std::string str);
	bool isAssign(std::string str);
	bool isExpr(std::string str);
	bool isFactor(std::string str);
	bool isVarName(std::string str);
	bool isConstValue(std::string str);
	*/

};