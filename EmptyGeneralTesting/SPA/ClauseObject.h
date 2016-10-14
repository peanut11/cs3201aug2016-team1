#pragma once
#include "ClauseType.h"

class ClauseObject {

protected:
	ClauseObject();
	ClauseType::ClauseType mClauseType;

public:
	//virtual ~ClauseObject();
	ClauseType::ClauseType getClauseType() const;
	virtual ClauseObject* clone()  = 0;

	/*
	bool operator<(const ClauseObject* rhs) const
	{
		

		if (this->getClauseType() == ClauseType::ClauseType::SUCH_THAT &&
			rhs->getClauseType() == ClauseType::ClauseType::SUCH_THAT) {

			const ClauseSuchThatObject* lhsObj = dynamic_cast<const ClauseSuchThatObject*>(this);
			const ClauseSuchThatObject* rhsObj = dynamic_cast<const ClauseSuchThatObject*>(rhs);

			if (lhsObj->getArgsOne()->getEntityType() == EntityType::VARIABLE ||
				lhsObj->getArgsTwo()->getEntityType() == EntityType::VARIABLE ||
				lhsObj->getArgsOne()->getEntityType() == EntityType::CONSTANT ||
				lhsObj->getArgsTwo()->getEntityType() == EntityType::CONSTANT) {

				return false;
			}

			if (lhsObj->getRelationshipType() == RelationshipType::MODIFIES ||
				lhsObj->getRelationshipType() == RelationshipType::USES) {
				return false;
			}

			if (rhsObj->getRelationshipType() != RelationshipType::MODIFIES &&
				rhsObj->getRelationshipType() != RelationshipType::USES) {
				return true;
			}

		}
		else {
			return this->getClauseType() < rhs->getClauseType();
		}
	}
	*/
};