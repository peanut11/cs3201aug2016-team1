// Maintained by: Aaron
//
// Used by:
// - ProgramConvertor
// - ProgramValidator

#pragma once

#include <string>
#include <deque>

class StringTokenizer {
	std::deque<std::string> tokens;
	bool isDelimiter(char ch);

public:
	StringTokenizer(std::string str);
	int countTokens();
	bool hasMoreTokens();
	std::string nextToken();
};