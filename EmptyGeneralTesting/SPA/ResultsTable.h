#pragma once
#include "ResultsObject.h"

class ResultsTable {
	std::set<ResultsObject> resultsObject;

public:
	ResultsObject getObject(SynonymString synonym);
	std::set<ResultsObject> getObjects();
	std::set<SynonymString> getSetString(SynonymString synonym);
	std::set<SynonymValue> getSetInt(SynonymString synonym);

	bool replaceSet(SynonymString synonym, std::set<SynonymString> newSet);
	bool replaceSet(SynonymString synonym, std::set<SynonymValue> newSet);
	bool insertSet(SynonymString synonym, std::set<SynonymString> newSet);
	bool insertSet(SynonymString synonym, std::set<SynonymValue> newSet);
	bool insert(ResultsObject object);
	bool contains(ResultsObject object);
	bool contains(SynonymString synonym);
	int size();
};