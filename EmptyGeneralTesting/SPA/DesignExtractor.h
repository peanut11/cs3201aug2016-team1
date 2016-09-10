#pragma once

#include <string>
#include <vector>

#include "PKB.h"

class DesignExtractor {
private:
	std::vector<StmtRow> stmtTable;

public:
	DesignExtractor();
	void DesignExtractor::process();
	void DesignExtractor::processFollowStar();
	void DesignExtractor::processParentStar();
	void DesignExtractor::updateStmtTable();
	void DesignExtractor::processWhileLoop(StmtNumber w);
	
};