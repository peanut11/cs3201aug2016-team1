#pragma once
#include <vector>

class QueryResultProjector {

	std::vector<std::string> projectedResults;

public:
	QueryResultProjector();
	std::vector<std::string> evaluate(std::vector<std::string> results);
};