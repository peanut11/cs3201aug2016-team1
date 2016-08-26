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
	static const std::string INVALID_CHARACTER;

public:
	StringTokenizer(std::string str);
	int countTokens();
	bool hasMoreTokens();
	std::string nextToken();
};