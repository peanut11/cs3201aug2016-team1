#pragma once

#include <set>
#include <string>
#include <vector>

#include "EntityType.h"
#include "Types.h"

class SynonymObject {
	EntityType type;
	SynonymString synonym;
	std::set<SynonymString> mSetString;
	std::set<SynonymValue> mSetInt;

public:
	
	SynonymObject();

	SynonymObject(EntityType type, std::string synonym);
	EntityType getType();

	std::set<SynonymString> getSetString();
	std::set<SynonymValue> getSetInt();
	std::string getSynonym();
	std::string getTypeString();

	void replaceSet(std::set<SynonymString> newSet);
	void replaceSet(std::set<SynonymValue> newSet);
	void insertSet(std::set<SynonymString> updatedSet);
	void insertSet(std::set<SynonymValue> updatedSet);
	void insertSet(SynonymString variable);
	void insertSet(SynonymValue stmtNum);

};