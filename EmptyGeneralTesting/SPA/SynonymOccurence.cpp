#include <map>
#include <string> 
#include "SynonymOccurence.h"


const int SynonymOccurence::MAX_SYNONYM_OCCURENCE = 2;

const int SynonymOccurence::INDEX_CLAUSE_SELECT = 0;
const int SynonymOccurence::INDEX_CLAUSE_SUCH_THAT = 1;
const int SynonymOccurence::INDEX_CLAUSE_PATTERN = 2;
const int SynonymOccurence::INDEX_CLAUSE_WITH = 3;

SynonymOccurence *SynonymOccurence::_instance;

SynonymOccurence *SynonymOccurence::getInstance() {
	if (!_instance)
		_instance = new SynonymOccurence;
	return _instance;
}

void SynonymOccurence::clearAll() {
	this->mMap.clear();
}

void SynonymOccurence::setIncrementNumberOccurence(std::string synonym, ClauseType::ClauseType type) {
	int oldNum = this->getNumberOccurence(synonym, type);
	int newNum = oldNum + 1;
	this->mMap[synonym][this->getClauseIndex(type)] = newNum;
	//this->mMap.insert(std::pair<std::string, int>(synonym, newNum));
}

std::string SynonymOccurence::toString() {
	std::string output = "";
	output.append("============ SYNONYM OCCURENCE RESULT ===============\n");
	for (std::map<std::string, int[4]>::iterator ii = this->mMap.begin(); ii != this->mMap.end(); ii++) {
		//output.append(ii->first + " - " + std::to_string(ii->second) + "\n");
		output.append(ii->first + " - ");
		for (auto value : ii->second) {
			output.append(" | " + std::to_string(value));
		}
		output.append("\n");
	}
	return output;
}

bool SynonymOccurence::hasMaximumOccurence(std::string synonym, ClauseType::ClauseType type) {
	
	std::map<std::string, int[4]>::iterator iter = this->mMap.find(synonym);
	if (iter != this->mMap.end()) {
		// found
		int total = 0;
		for (auto value : iter->second) {
			total += value;
		}

		return total == MAX_SYNONYM_OCCURENCE;
	}

	return 0;

	/*
	if (this->getNumberOccurence(synonym, type) == MAX_SYNONYM_OCCURENCE) {
		return true;
	}
	return false;
	*/

}

int SynonymOccurence::getNumberOccurence(std::string synonym, ClauseType::ClauseType type) {
	std::map<std::string, int[4]>::iterator iter = this->mMap.find(synonym);
	if (iter != this->mMap.end()){
		// found
		return iter->second[this->getClauseIndex(type)];
	}

	return 0;
}

int SynonymOccurence::getClauseIndex(ClauseType::ClauseType type) {
	switch (type) {
	case ClauseType::ClauseType::SELECT:
		return INDEX_CLAUSE_SELECT;

	case ClauseType::ClauseType::SUCH_THAT:
		return INDEX_CLAUSE_SUCH_THAT;

	case ClauseType::ClauseType::PATTERN:
		return INDEX_CLAUSE_PATTERN;

	case ClauseType::ClauseType::WITH:
		return INDEX_CLAUSE_WITH;
	default:
		return 0;
	}
}