#include "ClauseWithObject.h"

ClauseWithObject::ClauseWithObject() {
	this->mClauseType = ClauseType::ClauseType::WITH;
}

ClauseWithObject::ClauseWithObject(ClauseWithRefObject* obj1, ClauseWithRefObject* obj2) {
	this->mClauseType = ClauseType::ClauseType::WITH;
	this->obj1 = obj1;
	this->obj2 = obj2;
}

ClauseWithRefObject* ClauseWithObject::getRefObject1()
{
	return obj1;
}

ClauseWithRefObject* ClauseWithObject::getRefObject2()
{
	return obj2;
}

void ClauseWithObject::setResultsBoolean(bool boolean)
{
	this->isResultsBoolean = boolean;
}

bool ClauseWithObject::getResultsBoolean()
{
	return this->isResultsBoolean;
}

bool ClauseWithObject::isSynonymContained(std::string synonym) {
	return this->getRefObject1()->getSynonym().compare(synonym) ||
		this->getRefObject2()->getSynonym().compare(synonym);
}
