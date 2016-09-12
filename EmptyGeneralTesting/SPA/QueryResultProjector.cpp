#include "QueryResultProjector.h"

QueryResultProjector::QueryResultProjector()
{
}

std::vector<std::string> QueryResultProjector::evaluate(std::vector<std::string> results)
{
	this->projectedResults = results;
	return this->projectedResults;
}
