#pragma once
enum EntityType {
    ASSIGN,
    CALL,
    IF,
    WHILE,
    STMT,

	PROCEDURE,
	PROGRAM_LINE,
	PLUS,
	MINUS,
	TIMES,
	VARIABLE, // String
	CONSTANT, // Integer
	WILDCARD,
	INVALID
};