#pragma once

#include <list>
#include "Types.h"

class ResultGrid {
private:
    static unsigned int gridCount;

    unsigned int gridIndex; 
    std::list<std::vector<SynonymValue>> resultGrid;

public:
    ResultGrid(SynonymString syn, std::set<SynonymValue> vals);

    unsigned int getGridIndex();
    void mergeGrid(ResultGrid other);
    void updateSynonym(SynonymString syn, std::set<SynonymValue> vals);
    void updateSynonymTuple(std::tuple<SynonymString> synTuple, std::set<std::tuple<SynonymValue>> valTuples);
    std::set<SynonymValue> getValuesForSynonym(SynonymString syn);
    std::set<SynonymValue> getValueTuplesForSynonymTuple(std::tuple<SynonymString> synTuple);
    std::set<std::tuple<SynonymValue>> getTuple(SynonymString syn1, SynonymString syn2);
};
