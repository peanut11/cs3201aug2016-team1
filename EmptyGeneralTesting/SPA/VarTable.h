#pragma once
#include <vector>

typedef int stmt_no;

class VarTable
{
public:
	VarTable();
	~VarTable();
private:
	std::vector<stmt_no> UsedBy;
	std::vector<stmt_no> ModifiedBy;
};

