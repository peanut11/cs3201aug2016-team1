#pragma once

#include "ResultGrid.h"

class ResultGridManager {
private:
    typedef std::array<unsigned int, 2> GridRef; // gridIndex, gridCol
    std::vector<ResultGrid> resultGrids;
    std::map<SynonymString, GridRef> gridRefMap; // getGridForSynonym, getGridColForSynonym
    std::vector<std::vector<SynonymString>> gridTable; // getSynonymsInGrid

public:
    ResultGrid createGridForSynonym(SynonymString syn, std::set<SynonymValue> vals);
    ResultGrid getGridForSynonym(SynonymString syn);
    
    bool areInSameGrid(SynonymString syn1, SynonymString syn2);
};
