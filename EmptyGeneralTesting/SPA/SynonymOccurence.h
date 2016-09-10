#pragma once
#include <map>
#include "ClauseType.h"

class SynonymOccurence {

	static SynonymOccurence *_instance;
	static const int MAX_SYNONYM_OCCURENCE;

	// clauses - select, such that, pattern, with
	static const int INDEX_CLAUSE_SELECT;
	static const int INDEX_CLAUSE_SUCH_THAT;
	static const int INDEX_CLAUSE_PATTERN;
	static const int INDEX_CLAUSE_WITH;

	std::map<std::string, int[4]> mMap;
	int getClauseIndex(ClauseType::ClauseType type);

public:

	static SynonymOccurence *getInstance();

	void clearAll();
	void setIncrementOccurence(std::string synonym, ClauseType::ClauseType type);
	
	std::string toString();
	
	bool hasMaxCommonSynonym();

	bool hasMaximumOccurence(std::string synonym, ClauseType::ClauseType type);
	int getNumberOccurence(std::string synonym, ClauseType::ClauseType type);


};

