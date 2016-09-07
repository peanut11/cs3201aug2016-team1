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

	std::set<std::string> getSetString();
	std::set<int> getSetInt();
	std::string getSynonym();
	std::string getTypeString();

	void replaceSet(std::set<std::string> newSet);
	void replaceSet(std::set<int> newSet);
	void insertSet(std::set<std::string> updatedSet);
	void insertSet(std::set<int> updatedSet);
	void insertSet(std::string variable);
	void insertSet(int stmtNum);

};