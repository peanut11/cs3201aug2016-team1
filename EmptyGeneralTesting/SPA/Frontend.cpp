// Maintained by: Aaron
//
// Accessed by:
// - AutoTester::TestWrapper::parse

#include "Frontend.h"

std::string Frontend::read(std::string filename) {
	std::string foldername = "../Release/"; // Used by AutoTester
	std::string filepath = foldername.append(filename);
	std::stringstream buffer;
	std::ifstream infile(filepath);

	if (infile.good()) {
		buffer << infile.rdbuf();
		infile.close();
		return buffer.str();
	}

	return std::string();
}

void Frontend::extractDesign() {
	DesignExtractor de = DesignExtractor();
	de.process();
}

Frontend::Frontend() {}

void Frontend::parse(std::string filename) {
	std::string sourceFile = read(filename);

	Parser parser = Parser();
	if (parser.isValidProgram(sourceFile)) {
		parser.convert(sourceFile);
		extractDesign();
	}
}
