#pragma once

enum RelationshipType {
	MODIFIES, 
	USES,

	CALLS,
	CALLED_BY, //CALLS + 1
	CALLSSTAR, 
	CALLED_BY_STAR, //CALLS_STAR + 1
	FOLLOWS,
	FOLLOWED_BY, // FOLLOWS + 1
	FOLLOWS_STAR,
	FOLLOWED_BY_STAR, // FOLLOWS_STAR + 1
	PARENT,
	PARENT_OF,   // PARENT + 1
	PARENT_STAR,
	PARENT_OF_STAR, // PARENT_STAR + 1

	TOTAL_COUNT,
	INVALID_RELATIONSHIP
};
