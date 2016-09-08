#include "ClausePatternObject.h"

ClausePatternObject::ClausePatternObject(EntityType entityType, std::string leftHand, std::string rightHand) {
	this->entityType = entityType;
	this->leftHand = leftHand;
	this->rightHand = rightHand;
}

EntityType ClausePatternObject::getEntityType()
{
	return entityType;
}

std::string ClausePatternObject::getLeftHand()
{
	return leftHand;
}

std::string ClausePatternObject::getRightHand()
{
	return rightHand;
}
