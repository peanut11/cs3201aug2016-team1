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

	std::string toString();
	bool insert(SynonymObject object);
	bool contains(SynonymObject object);
	bool contains(std::string synonym);
	int size();
};