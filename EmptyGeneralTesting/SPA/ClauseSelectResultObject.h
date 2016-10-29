#pragma once
#include "ClauseSelectObject.h"
#include <vector>

class ClauseSelectResultObject : public ClauseObject {

	std::vector<ClauseSelectObject> mClauseSelectObjList;
	bool isBoolean = false;
	
	

public:
	ClauseSelectResultObject();
	void setBoolean(bool value);
	void insertClauseSelectObject(ClauseSelectObject object);
	void clearAll();

	ClauseSelectResultObject* clone() { return new ClauseSelectResultObject(*this); }

	std::vector<ClauseSelectObject> getClauseSelectObjectList();
	bool doesClauseSelectObjectExist(std::string synonym);
	bool getBoolean();

	int size();



};


