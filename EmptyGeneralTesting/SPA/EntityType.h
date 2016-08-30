#pragma once
enum EntityType {
	PROCEDURE,		// procedure
	PROGRAM_LINE,	// program line
	STMT,		// statement
	ASSIGN,		// assignment
	CALL,		// call 
	WHILE,		// while
	IF,			// if
	PLUS,
	MINUS,
	TIMES,
	VARIABLE,			// variable (string)
	CONSTANT,			// constant (integer)
	WILDCARD			// wildcard
};