// Maintained by: Aaron & Ngoc Khanh
//
// Facade of:
// - ProgramValidator
// - ProgramConverter

#pragma once

#include "Parser.h"

Parser::Parser () {
	mConverter = ProgramConverter();
	mValidator = ProgramValidator();
}

bool Parser::isValidProgram(std::string str)
{
	return mValidator.isValidSyntax(str);
}

int Parser::convert(std::string source)
{
	return 0;
}
