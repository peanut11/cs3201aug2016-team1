#pragma once

#include <string>
#include <vector>

#include "PKB.h"

class DesignExtractor {


public:
	DesignExtractor();
	void DesignExtractor::dfs(StmtNumber index, bool *visited,StmtNumber caller);
	void DesignExtractor::processCallsStar();
	void DesignExtractor::process();
	void DesignExtractor::processFollowedByStar();
	void DesignExtractor::processPARENT_STAR();
	void DesignExtractor::updateStmtTable();
	void DesignExtractor::processLoopForUseAndModifies(StmtNumber w);
	void DesignExtractor::processParentOfStar();
	void DesignExtractor::processFollowsStar();
	StmtNumber DesignExtractor::getwhileList();
	
};
