#pragma once
#include<stdio.h>
#include <vector>
#include "RelObjectType.h";
#include "RelArgsType.h";


class RelObject {
	
	RelObjectType type;
	std::vector<RelArgsType> argsOneTypes;
	std::vector<RelArgsType> argsTwoTypes;
	int numOfArgs;

public:

	RelObject(RelObjectType type, std::vector<RelArgsType> argsOneTypes, std::vector<RelArgsType> argsTwoTypes, int numOfArgs) {
		this->type = type;
		this->argsOneTypes = argsOneTypes;
		this->argsTwoTypes = argsTwoTypes;
		this->numOfArgs = numOfArgs;
	}

	RelObjectType getRelObjectType() {
		return this->type;
	}

	std::vector<RelArgsType> getArgsOneTypes() {
		return this->argsOneTypes;
	}

	std::vector<RelArgsType> getArgsTwoTypes() {
		return this->argsTwoTypes;
	}

	int getNumOfArgs() {
		return this->numOfArgs;
	}

};
