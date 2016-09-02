// Maintained by: Aaron
//
// Used by:
// - ProgramConvertor
// - ProgramValidator

#pragma once

#include <string>
#include <deque>

enum DelimiterMode {
	PARSER,
	QUERY_PREPROCESSOR
};

class StringTokenizer {
	static const std::string EMPTY_TOKEN;

	std::deque<std::string> tokens;
	std::string delimiters;
	bool isDelimiter(char ch);

public:
	StringTokenizer(std::string str, DelimiterMode mode);
	int countTokens();
	bool hasMoreTokens();
	std::string nextToken();

	std::string peekNextToken();
	void popNextToken();
	void returnToken(std::string);
};
