#pragma once

#include <list>
#include "Types.h"

typedef unsigned int TuplePosition;
typedef unsigned int GridIndexOrCol;
typedef GridIndexOrCol GridIndex;
typedef GridIndexOrCol GridColumn;
typedef std::map<SynonymString, GridIndex> GridIndexMap;
typedef std::map<SynonymString, GridColumn> GridColumnMap;
typedef std::tuple<SynonymString, SynonymString> SynonymTuple;
typedef std::tuple<SynonymValue, SynonymValue> ValueTuple;
typedef std::vector<SynonymValue> GridRow;
typedef std::set<SynonymValue> ValueSet;
typedef std::set<ValueTuple> ValueTupleSet;
typedef std::list<GridRow>::iterator GridListIterator;

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
