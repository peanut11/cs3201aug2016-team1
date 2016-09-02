#include "stdafx.h"
#include "Tools.h"

#include <fstream>

std::string Tools::readFile(std::string filename) {
	std::string foldername = "Testcases/";
	std::string filepath = foldername.append(filename);
	std::stringstream buffer;
	std::ifstream infile;

	infile.open(filepath);
	buffer << infile.rdbuf();
	infile.close();

	return buffer.str();
}
