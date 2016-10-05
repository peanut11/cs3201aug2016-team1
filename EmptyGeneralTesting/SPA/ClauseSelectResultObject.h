#pragma once
#include "ClauseSelectObject.h"
#include <vector>

class ClauseSelectResultObject : public ClauseObject {

	std::vector<ClauseSelectObject> mClauseSelectObjList;
	bool isBoolean;
	
	ClauseSelectResultObject();

public:
	
	void insertClauseSelectObject(ClauseSelectObject object);
	

	std::vector<ClauseSelectObject> getClauseSelectObjectList();
	bool doesClauseSelectObjectExist(std::string synonym);
	bool getBoolean();




};


