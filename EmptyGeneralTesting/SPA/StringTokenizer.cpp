// Maintained by: Aaron

#pragma once

#include <cctype>
#include "StringTokenizer.h"

const std::string StringTokenizer::INVALID_CHARACTER = "Invalid character: ";

StringTokenizer::StringTokenizer(std::string str) {
	const std::string delimiters = "{=+-*};\n";
	const std::string space = " ";
	std::string buffer = "";

	for (unsigned int i = 0; i < str.length(); i++) {
		std::string ch = std::string(1, str.at(i));
		bool chIsDelimiter = (delimiters.find(ch) != std::string::npos);

		if (chIsDelimiter || ch == space) {
			if (buffer.length() != 0) {
				tokens.push_back(buffer);
				buffer.clear();
			}

			if (chIsDelimiter) {
				tokens.push_back(ch);
			}
		} else {
			char character = ch.at(0);
			if (std::isalnum(character)) {
				buffer.append(ch);
			} else {
				throw std::runtime_error(INVALID_CHARACTER + ch);
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
