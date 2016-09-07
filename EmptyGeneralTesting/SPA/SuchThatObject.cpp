#include "SuchThatObject.h"

std::vector<SuchThatRelObject> SuchThatObject::getRelationships()
{
	return relationships;
}

void SuchThatObject::evaluate()
{
	// iterate all the relationships
	for (std::vector<SuchThatRelObject>::iterator it = relationships.begin(); it < relationships.end(); it++) {

	}
}
