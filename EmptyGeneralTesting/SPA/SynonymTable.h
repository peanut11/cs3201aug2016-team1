#pragma once
#include <vector>
#include "SynonymObject.h"

class SynonymTable {
	static SynonymTable* _instance;
	std::vector<SynonymObject> synonymObjects;

public:
	static SynonymTable *getInstance();

	SynonymObject getObject(int index);
	int insert(SynonymObject object);
	int size();
};