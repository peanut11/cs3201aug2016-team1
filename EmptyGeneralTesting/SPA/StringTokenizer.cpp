// Maintained by: Aaron

#pragma once

#include "StringTokenizer.h"

const std::string StringTokenizer::EMPTY_TOKEN = "";

bool StringTokenizer::getIsIgnoreNewlines() {
    return isIgnoreNewlines;
}

bool StringTokenizer::isNewline(char ch) {
    return ch == '\n';
}

bool StringTokenizer::isDelimiter(char ch) {
	return delimiters.find(ch) != std::string::npos;
}

StringTokenizer::StringTokenizer(std::string str, DelimiterMode mode) {
	const std::string WHITESPACE = " ";
    isIgnoreNewlines = false;
	
	switch(mode) {
	case PARSER:
		delimiters = "({=+-*});\n";
		break;
	case QUERY_PREPROCESSOR:
		delimiters = "*<>(#,._+=);\n\"";
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
                if (isIgnoreNewlines && isNewline(ch)) {
                    continue;
                }
				tokens.push_back(charString);
			}
		} else {
			if (std::isalnum(ch)) {
				buffer.append(charString);
			} else {
				throw Exceptions::invalid_character(charString);
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
