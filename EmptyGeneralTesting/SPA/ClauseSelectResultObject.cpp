#include "ClauseSelectResultObject.h"
#include "ClauseType.h"

ClauseSelectResultObject::ClauseSelectResultObject() {
	this->mClauseType == ClauseType::ClauseType::RESULT;
}

void ClauseSelectResultObject::insertClauseSelectObject(ClauseSelectObject newObject) {
	this->mClauseSelectObjList.push_back(newObject);
}


std::vector<ClauseSelectObject> ClauseSelectResultObject::getClauseSelectObjectList() {
	return this->mClauseSelectObjList;
}

bool ClauseSelectResultObject::doesClauseSelectObjectExist(std::string synonymKey) {
	for (auto value : this->mClauseSelectObjList) {
		if (value.getSynonymString().compare(synonymKey) == 0) {
			return true;
		}
	}
	return false;
}

bool ClauseSelectResultObject::getBoolean() {
	return this->isBoolean;
}

int ClauseSelectResultObject::size() {
	return this->mClauseSelectObjList.size();
}
