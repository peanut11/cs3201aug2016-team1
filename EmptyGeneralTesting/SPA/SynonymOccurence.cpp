#include <map>
#include <string> 
#include "SynonymOccurence.h"


const int SynonymOccurence::MAX_SYNONYM_OCCURENCE = 2;

SynonymOccurence *SynonymOccurence::_instance;

SynonymOccurence *SynonymOccurence::getInstance() {
	if (!_instance)
		_instance = new SynonymOccurence;
	return _instance;
}

void SynonymOccurence::clearAll() {
	this->mMap.clear();
}

void SynonymOccurence::setIncrementNumberOccurence(std::string synonym) {
	int oldNum = this->getNumberOccurence(synonym);
	int newNum = oldNum + 1;
	this->mMap.insert(std::pair<std::string, int>(synonym, newNum));
}

std::string SynonymOccurence::toString() {
	std::string output = "";
	output.append("============ SYNONYM OCCURENCE RESULT ===============\n");
	for (std::map<std::string, int>::iterator ii = this->mMap.begin(); ii != this->mMap.end(); ii++) {
		output.append(ii->first + " - " + std::to_string(ii->second) + "\n");
	}
	return output;
}

bool SynonymOccurence::hasMaximumOccurence(std::string synonym) {
	if (this->getNumberOccurence(synonym) == MAX_SYNONYM_OCCURENCE) {
		return true;
	}
	return false;
}

int SynonymOccurence::getNumberOccurence(std::string synonym) {
	std::map<std::string, int>::iterator iter = this->mMap.find(synonym);
	if (iter != this->mMap.end()){
		// found
		return iter->second;
	}

	return 0;
}