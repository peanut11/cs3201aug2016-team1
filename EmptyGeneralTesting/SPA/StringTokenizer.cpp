// Maintained by: Aaron

#pragma once

#include <cctype>
#include "StringTokenizer.h"

bool StringTokenizer::isDelimiter(char ch) {
	const std::string DELIMITERS = "{=+-*};\n";
	return DELIMITERS.find(ch) != std::string::npos;
}

StringTokenizer::StringTokenizer(std::string str) {
	const std::string WHITESPACE = " ";
	std::string buffer = "";

	for (unsigned int i = 0; i < str.length(); i++) {
		char ch = str.at(i);
		std::string charString = std::string(1, ch);

		if (std::isspace(ch) || isDelimiter(ch)) {
			if (buffer.length() != 0) {
				tokens.push_back(buffer);
				buffer.clear();
			}
			if (isDelimiter(ch)) {
				tokens.push_back(charString);
			}
		} else {
			if (std::isalnum(ch)) {
				buffer.append(charString);
			} else {
				throw std::runtime_error("Invalid character: " + charString);
			}
		}
	}
}

int StringTokenizer::countTokens() {
	return tokens.size();
}

bool StringTokenizer::hasMoreTokens() {
	return !tokens.empty();
}

std::string StringTokenizer::nextToken() {
	if (!tokens.empty()) {
		const std::string NEXT_TOKEN = tokens.front();
		tokens.pop_front();
		return NEXT_TOKEN;

	} else {
		const std::string EMPTY_TOKEN = "";
		return EMPTY_TOKEN;
	}
}
