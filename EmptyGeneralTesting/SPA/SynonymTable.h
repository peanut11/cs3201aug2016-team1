#pragma once
#include <vector>
#include "SynonymObject.h"

class SynonymTable {
	static SynonymTable* _instance;
	std::vector<SynonymObject> synonymObjects;
	SynonymObject invalidObject = SynonymObject(EntityType::INVALID, "");

public:
	static SynonymTable *getInstance();

	void clearAll();

	SynonymObject getObject(std::string synonym);
	SynonymObject getObject(int index);
	std::vector<SynonymObject> getObjects();

	std::set<std::string> getSetString(std::string synonym);
	std::set<int> getSetInt(std::string synonym);

	std::string toString();
	bool replaceSet(std::string synonym, std::set<std::string> newSet);
	bool replaceSet(std::string synonym, std::set<int> newSet);
	bool insertSet(std::string synonym, std::set<std::string> newSet);
	bool insertSet(std::string synonym, std::set<int> newSet);
	bool insert(SynonymObject object);
	bool contains(SynonymObject object);
	bool contains(std::string synonym);
	int size();
};