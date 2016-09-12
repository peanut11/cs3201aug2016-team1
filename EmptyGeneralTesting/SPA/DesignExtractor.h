#pragma once

#include <string>
#include <vector>

#include "PKB.h"

class DesignExtractor {


public:
	DesignExtractor();
	void DesignExtractor::process();
	void DesignExtractor::processFollowedByStar();
	void DesignExtractor::processParentStar();
	void DesignExtractor::updateStmtTable();
	void DesignExtractor::processWhileLoop(StmtNumber w);
	void DesignExtractor::processParentOfStar();
	void DesignExtractor::processFollowsStar();
	StmtNumber DesignExtractor::getwhileList();
	
};
