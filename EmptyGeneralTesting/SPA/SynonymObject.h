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
			return "Procedure";
		case ASSIGN:
			return "Assign";
		}
	}
};