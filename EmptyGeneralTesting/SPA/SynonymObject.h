#pragma once
#include <string>

#include "RelArgsType.h"

class SynonymObject {
	RelArgsType type;
	std::string synonym;

public:
	
	SynonymObject(RelArgsType type, std::string synonym) {
		this->type = type;
		this->synonym = synonym;
	}

	RelArgsType getType() {
		return this->type;
	}

	std::string getSynonym() {
		return this->synonym;
	}
};