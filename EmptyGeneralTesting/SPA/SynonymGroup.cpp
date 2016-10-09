#include <string> 
#include <algorithm>
#include "SynonymGroup.h"


SynonymGroup *SynonymGroup::getInstance() {
	if (!_instance)
		_instance = new SynonymGroup;
	return _instance;
}

void SynonymGroup::clearAll() {
	this->mapSynonymToGroupIndex.clear();
	this->mapGroupIndexToSynonyms.clear();
}

void SynonymGroup::insertSynonym(std::string synonym) {
	if (synonym.compare("") != 0) {
		// first check whether synonym has already inserted
		if (!this->containSynonym(synonym)) {
			this->insertSynonym(synonym, NEW_GROUP_INDEX);

			NEW_GROUP_INDEX += 1;
		}
	}
}

void SynonymGroup::insertSynonym(std::string synonym, int groupIndex) {
	if (synonym.compare("") != 0) {

		if (!this->containSynonym(synonym)) {
			this->mapSynonymToGroupIndex[synonym] = groupIndex;
			this->mapGroupIndexToSynonyms[groupIndex].push_back(synonym);
		}
		else {
			this->mapSynonymToGroupIndex[synonym] = groupIndex;
			this->updateAllSynonyms(this->mapGroupIndexToSynonyms[groupIndex], groupIndex);
		}
		

		

		
	}
}

void SynonymGroup::updateAllSynonyms(std::vector<std::string> synonyms, int newGroupIndex) {
	for (auto value : synonyms) {

		int oldGroupIndex = this->mapSynonymToGroupIndex[value]; // get old index of the synonym first

		this->mapSynonymToGroupIndex[value] = newGroupIndex; // update the new index
		this->mapGroupIndexToSynonyms[newGroupIndex].push_back(value); // insert synonym into specific group index

		std::vector<std::string>::iterator position = std::find(this->mapGroupIndexToSynonyms[oldGroupIndex].begin(), this->mapGroupIndexToSynonyms[oldGroupIndex].end(), value);
		if (position != this->mapGroupIndexToSynonyms[oldGroupIndex].end()) // == end() means the element was not found
			this->mapGroupIndexToSynonyms[oldGroupIndex].erase(position); // remove the synonym from old group index
	}
}

std::string SynonymGroup::toString() {
	std::string output = "";
	output.append("============ SYNONYM GROUP RESULT ===============\n");
	output.append("\tS\tST\tP\tW\n");
	for (std::map<std::string, int>::iterator ii = this->mapSynonymToGroupIndex.begin(); ii != this->mapSynonymToGroupIndex.end(); ii++) {

		output.append(ii->first + ":");
		output.append("\t" + std::to_string(ii->second));
		output.append("\n");
	}
	return output;
}

bool SynonymGroup::containSynonym(std::string synonym) {
	std::map<std::string, int>::iterator iter = this->mapSynonymToGroupIndex.find(synonym);
	if (iter != this->mapSynonymToGroupIndex.end()) {
		return true;
	}
	return false;
}

int SynonymGroup::getGroupIndex(std::string synonym) {
	std::map<std::string, int>::iterator iter = this->mapSynonymToGroupIndex.find(synonym);
	if (iter != this->mapSynonymToGroupIndex.end()) {
		// found
		return iter->second;
	}

	return -1;
}

