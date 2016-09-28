#pragma once

#include "Exceptions.h"
#include "ResultGrid.h"

class ResultGridManager {
private:
    GridIndexMap refMap;                               // getGridIndexForSynonym
    std::vector<ResultGrid*> refTable;                 // getGridByIndex
    std::vector<std::vector<SynonymString>> gridTable; // getSynonymsInGrid

    ResultGrid* getGridForSynonymTuple(std::tuple<SynonymString> synTuple);
    GridIndex extractGridIndex(GridRefMap::const_iterator it);
    GridIndex getGridIndexForSynonym(SynonymString syn);
    ResultGrid* createGridForSynonym(SynonymString syn, ValueSet vals);
    ResultGrid* getGridForSynonym(SynonymString syn);

public:
    bool areInSameGrid(SynonymString syn1, SynonymString syn2);
    void initialiseValuesForSynonym(SynonymString syn, ValueSet vals);
    void updateSynonym(SynonymString syn, ValueSet vals);
    void updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet valTuples);
    ValueSet getValuesForSynonym(SynonymString syn);
    ValueTupleSet getValuesForSynonymTuple(SynonymTuple synTuple);
};
