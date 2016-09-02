#pragma once

#include "AssignmentTree.h"
#include "PKB.h"
#include <string>

class ProgramConverter {
public:
	ProgramConverter();
	void convert(std::string, AssignmentTree&, VarTable&);
};
