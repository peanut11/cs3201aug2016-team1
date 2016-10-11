#pragma once
#include <map>
#include <vector>

class SynonymGroup {

	static SynonymGroup *_instance;

	std::map<std::string, int> mapSynonymToGroupIndex;
	std::map<int, std::vector<std::string>> mapGroupIndexToSynonyms;

	void updateAllSynonyms(std::vector<std::string> synonyms, int oldGroupIndex, int newGroupIndex);


public:
	static SynonymGroup *getInstance();
	int NEW_GROUP_INDEX = 1;

	void clearAll();
	void insertSynonym(std::string synonym);
	void insertSynonym(std::string synonym, int groupIndex);

	std::string toString();
	bool containSynonym(std::string synonym);
	std::map<int, std::vector<std::string>> getMapGroupIndexToSynonyms();
	int getGroupIndex(std::string synonym);
	std::vector<std::string> getSynonyms(int groupIndex);


};