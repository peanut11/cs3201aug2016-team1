#pragma once

#include "EntityType.h"
#include "Types.h"
#include <set>

class ResultsObject {
	SynonymString synonym;
	std::set<SynonymString> mSetString;
	std::set<SynonymValue> mSetInt;

public:
	ResultsObject();
	ResultsObject(std::string synonym);

	std::set<SynonymString> getSetString();
	std::set<SynonymValue> getSetInt();
	std::string getSynonym();

	void replaceSet(std::set<SynonymString> newSet);
	void replaceSet(std::set<SynonymValue> newSet);
	void insertSet(std::set<SynonymString> updatedSet);
	void insertSet(std::set<SynonymValue> updatedSet);
//	void insertSet(SynonymString variable);
//	void insertSet(SynonymValue stmtNum);
};
