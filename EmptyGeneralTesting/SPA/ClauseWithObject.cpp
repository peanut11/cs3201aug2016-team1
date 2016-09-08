#include "ClauseWithObject.h"

ClauseWithObject::ClauseWithObject() {
}

ClauseWithObject::ClauseWithObject(ClauseWithRefObject obj1, ClauseWithRefObject obj2) {
	this->obj1 = obj1;
	this->obj2 = obj2;
}

ClauseWithRefObject ClauseWithObject::getRefObject1()
{
	return obj1;
}

ClauseWithRefObject ClauseWithObject::getRefObject2()
{
	return obj2;
}
