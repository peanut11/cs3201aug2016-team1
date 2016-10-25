#pragma once
#include <string>
#include "ClauseType.h"

class ClauseObject {

protected:
	ClauseObject();
	ClauseType::ClauseType mClauseType;

public:
	//virtual ~ClauseObject();
	ClauseType::ClauseType getClauseType() const;
	virtual ClauseObject* clone()  = 0;

	bool isSynonymContained(std::string synonym);

};