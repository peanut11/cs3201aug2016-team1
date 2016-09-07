#pragma once
#include <set>
#include <vector>
#include <string>

#include "EntityType.h"

class SynonymObject {
	EntityType type;
	std::string synonym;
	std::set<std::string> mSetString;
	std::set<int> mSetInt;

public:
	
	SynonymObject() {}

	SynonymObject(EntityType type, std::string synonym);
	EntityType getType();

	std::string getSynonym();
	std::string getTypeString();

	void insertSet(std::string variable);
	void insertSet(int stmtNum);

};