#pragma once
#include <map>

class SynonymOccurence {

	static SynonymOccurence *_instance;
	static const int MAX_SYNONYM_OCCURENCE;

	std::map<std::string, int> mMap;

public:

	static SynonymOccurence *getInstance();

	void setIncrementNumberOccurence(std::string synonym);
	
	std::string toString();
	
	bool hasMaximumOccurence(std::string synonym);
	int getNumberOccurence(std::string synonym);


};

