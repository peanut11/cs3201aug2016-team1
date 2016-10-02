#pragma once

#include "Exceptions.h"
#include "ResultGrid.h"

class ResultGridManager {
private:
    GridIndexMap refMap;                               // getGridIndexForSynonym
    std::vector<ResultGrid*> refTable;                 // getGridByIndex
    std::vector<std::vector<SynonymString>> gridTable; // getSynonymsInGrid

    bool createGridForSynonym(SynonymString syn, ValueSet vals); 
    GridIndex getGridIndexForSynonym(SynonymString syn);
    ResultGrid* getGridByIndex(GridIndex index);
    ResultGrid* getGridForSynonym(SynonymString syn);
    ValueTupleSet permutate(ValueSet vals1, ValueSet vals2);

public:
    bool areInSameGrid(SynonymString syn1, SynonymString syn2);
    bool initialiseSynonym(SynonymString syn, ValueSet vals);
    void updateSynonym(SynonymString syn, ValueSet vals);
    void updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet valTuples);
    ValueSet getValuesForSynonym(SynonymString syn);
    ValueTupleSet getValuesForSynonymTuple(SynonymTuple synTuple);
};