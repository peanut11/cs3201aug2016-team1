// Maintained by: Aaron
//
// Used by:
// - ProgramConvertor
// - ProgramValidator

#pragma once

#include <string>
#include <deque>

class StringTokenizer {
	static const std::string EMPTY_TOKEN;

	std::deque<std::string> tokens;
	bool isDelimiter(char ch);

public:
	StringTokenizer(std::string str);
	int countTokens();
	bool hasMoreTokens();
	std::string nextToken();

	std::string peekNextToken();
	void popNextToken();
	void returnToken(std::string);
};
