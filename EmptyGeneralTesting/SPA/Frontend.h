#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "Parser.h"
#include "DesignExtractor.h"

class Frontend {
private:
	std::string read(std::string filename);
	void extractDesign();

public:
	Frontend();
	void parse(std::string filename);
};
