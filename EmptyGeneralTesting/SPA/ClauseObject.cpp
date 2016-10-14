#include "ClauseObject.h"

ClauseObject::ClauseObject() {
}

/*
ClauseObject::~ClauseObject() {
}
*/

ClauseType::ClauseType ClauseObject::getClauseType() const {
	return this->mClauseType;
}

