#pragma once

#include <string>
#include <vector>

#include "PKB.h"

class DesignExtractor {
private:
    void dfs(StmtNumber index, bool *visited, StmtNumber caller);

    void processCallsForUsesAndModifies();
    void processCallsStar();
    void processFollowsStar();
    void processLoopForUsesAndModifies(StmtNumber w);
    void processParentStar(StmtNumber stmt);
    void updateStmtTable();

public:
    DesignExtractor();

    size_t getWhileListSize();
    void process();
};
