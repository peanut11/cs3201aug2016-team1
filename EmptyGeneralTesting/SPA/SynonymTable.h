#pragma once
#include <vector>
#include "SynonymObject.h"

class SynonymTable {
	static SynonymTable* _instance;
	std::vector<SynonymObject> synonymObjects;

public:
	static SynonymTable *getInstance();

	void clearAll();

	SynonymObject getObject(int index);
	std::string toString();
	int insert(SynonymObject object);
	int size();
};