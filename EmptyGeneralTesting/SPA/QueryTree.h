#pragma once
#include <vector>

class QueryTree {

	// store result synonm from query into a vector
	std::vector<std::string> result;

	// store relationships query from query into a vector
	std::vector<std::string> relationship;

	// store the type of results expected (procedure (p) - names, statement(s) - integer, ..)
	void storeResultType();

	// store the relationships needed to evaluate in the PQL queries (etc. Calls(p,q), Modifies(_,"x"), ......)
	void storeRelationships();



public:
	std::vector<std::string> getResultType();
	std::vector<std::string> getRelationships();

};
