#pragma once

#include "Exceptions.h"
#include "SynonymObject.h"

class SynonymTable {
	static SynonymTable* _instance;
	std::vector<SynonymObject> synonymObjects;
	SynonymObject invalidObject = SynonymObject(EntityType::INVALID, "");

public:
	static SynonymTable *getInstance();

	void clearAll();

	SynonymObject getObject(SynonymString synonym);
	SynonymObject getObject(SynonymValue index);
	EntityType getType(SynonymString synonym);
	std::vector<SynonymObject> getObjects();

	std::set<SynonymString> getSetString(SynonymString synonym);
	std::set<SynonymValue> getSetInt(SynonymString synonym);

	std::string toString();
	bool replaceSet(SynonymString synonym, std::set<SynonymString> newSet);
	bool replaceSet(SynonymString synonym, std::set<SynonymValue> newSet);
	bool replaceSet(SynonymValue synonym, std::set<SynonymString> newSet);
	bool insertSet(SynonymString synonym, std::set<SynonymString> newSet);
	bool insertSet(SynonymString synonym, std::set<SynonymValue> newSet);
	bool insert(SynonymObject object);
	bool contains(SynonymObject object);
	bool contains(SynonymString synonym);
	int size();
};
