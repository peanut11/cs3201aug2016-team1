#pragma once
#include <string>

#include "EntityType.h"

class SynonymObject {
	EntityType type;
	std::string synonym;

public:
	
	SynonymObject() {}

	SynonymObject(EntityType type, std::string synonym) {
		this->type = type;
		this->synonym = synonym;
	}

	EntityType getType() {
		return this->type;
	}

	std::string getSynonym() {
		return this->synonym;
	}

	std::string getTypeString() {
		switch (this->type) {
		case PROCEDURE:
			return "procedure";
		case ASSIGN:
			return "assign";
		case IF:
			return "if";
		case WHILE:
			return "while";
		case STMT:
			return "stmt";
		case VARIABLE:
			return "variable";
		case CONSTANT:
			return "constant";
		case PROGRAM_LINE:
			return "prog_line";
		case CALL:
			return "call";
		}
	}
};