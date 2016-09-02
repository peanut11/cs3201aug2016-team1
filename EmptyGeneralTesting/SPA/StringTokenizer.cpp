// Maintained by: Aaron

#pragma once

#include <cctype>
#include "StringTokenizer.h"

const std::string StringTokenizer::EMPTY_TOKEN = "";

bool StringTokenizer::isDelimiter(char ch) {
	return delimiters.find(ch) != std::string::npos;
}

StringTokenizer::StringTokenizer(std::string str, DelimiterMode mode) {
	const std::string WHITESPACE = " ";
	
	switch(mode) {
	case PROGRAM_VALIDATOR:
		delimiters = "{=+-*};\n";
		break;
	case QUERY_PREPROCESSOR:
		delimiters = "(,.);\n\"";
		break;
	default:
		delimiters = "";
		break;
	}

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

	if (!buffer.empty()) {
		tokens.push_back(buffer);
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
		return EMPTY_TOKEN;
	}
}

std::string StringTokenizer::peekNextToken() {
	if (!tokens.empty()) {
		return tokens.front();
	} else {
		return EMPTY_TOKEN;
	}
}

void StringTokenizer::popNextToken() {
	if (!tokens.empty()) {
		tokens.pop_front();
	}
}

void StringTokenizer::returnToken(std::string token) {
	tokens.push_front(token);
}
