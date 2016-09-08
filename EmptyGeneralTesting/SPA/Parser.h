#pragma once
#include <string>
#include "ProgramConverter.h"
#include "ProgramValidator.h"

class Parser {
	ProgramConverter mConverter;
	ProgramValidator mValidator;
public: 
	Parser();
	bool isValidProgram(std::string str);
	int convert(std::string source);
};
