#pragma once

#include "Exceptions.h"
#include "ResultGrid.h"

class ResultGridManager {
private:
    typedef unsigned int GridIndexOrCol;
    typedef GridIndexOrCol GridIndex;
    typedef GridIndexOrCol GridColumn;
    typedef std::tuple<GridIndex, GridColumn> GridRefTuple;
    typedef std::map<SynonymString, GridRefTuple> GridRefMap;
    
    GridRefMap refMap;                                 // getGridIndexForSynonym, getGridColForSynonym
    std::vector<ResultGrid*> refTable;                 // getGridByIndex
    std::vector<std::vector<SynonymString>> gridTable; // getSynonymsInGrid

    ResultGrid* createGridForSynonym(SynonymString syn, std::set<SynonymValue> vals);
    ResultGrid* getGridForSynonym(SynonymString syn);
    ResultGrid* getGridForSynonymTuple(std::tuple<SynonymString> synTuple);
    GridIndex extractGridIndex(GridRefMap::const_iterator it);
    GridIndex getGridIndexForSynonym(SynonymString syn);

public:
    bool areInSameGrid(SynonymString syn1, SynonymString syn2);
    void initialiseValuesForSynonym(SynonymString syn, std::set<SynonymValue> vals);
    void updateSynonym(SynonymString syn, std::set<SynonymValue> vals);
    void updateSynonymTuple(std::tuple<SynonymString> synTuple, std::set<std::tuple<SynonymValue>> valTuples);
    std::set<SynonymValue> getValuesForSynonym(SynonymString syn);
    std::set<std::tuple<SynonymValue>> getValuesForSynonymTuple(std::tuple<SynonymString> synTuple);
};
