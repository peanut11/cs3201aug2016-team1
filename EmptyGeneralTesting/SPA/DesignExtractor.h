#pragma once

#include <string>
#include <vector>

#include "PKB.h"

class DesignExtractor {
private:
    void dfs(StmtNumber index, bool *visited, StmtNumber caller);

public:
    DesignExtractor();

    size_t getWhileListSize();
    void process();
    void processCallsStar();
    void processFollowsStar();
    void processLoopForUseAndModifies(StmtNumber w);
    void processParentStar();
    void updateStmtTable();
};
