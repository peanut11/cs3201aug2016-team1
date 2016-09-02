#pragma once

#include "AbstractSyntaxTree.h"
#include "VarTable.h"
#include "../source/PKB.h"
#include <string>

class ProgramConverter {
public:
	ProgramConverter();
	void convert(std::string, AbstractSyntaxTree&, VarTable&);
};
