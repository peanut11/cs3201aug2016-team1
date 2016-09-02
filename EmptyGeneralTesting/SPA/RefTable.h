#pragma once
#include <string>

class RefTable
{
public:
	RefTable();
	~RefTable();
private:
	std::string varName;
	int index;
};

