#pragma once
enum RelArgsType {
	SYNONYM_PROC,		// procedure
	SYNONYM_PROG_LINE,	// program line
	SYNONYM_STMT,		// statement
	SYNONYM_ASSIGN,		// assignment
	SYNONYM_CALL,		// call 
	SYNONYM_WHILE,		// while
	SYNONYM_IF,			// if 
	STRING,				// strings
	INTEGER,			// integer
	UNDERSCORE			// wildcard
};