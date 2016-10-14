#pragma once

#include <algorithm> // std::stable_sort
#include <list>

#include "Exceptions.h"
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
typedef GridColumnMap::const_iterator GridMapConstIter;

class ResultGrid {
private:
    const TuplePosition LEFT = 0;
    const TuplePosition RIGHT = 0;

    GridColumnMap refMap;              // getColumnForSynonym
    std::vector<SynonymString> refTable; // getSynonymForColumn
    std::list<GridRow> resultList;     // Possible combinations of SynonymValues
    std::vector<ValueSet> resultTable; // Unique SynonymValues for each SynonymString

    bool contains(ValueSet valSet, SynonymValue val);
    bool contains(ValueTupleSet valTupleSet, ValueTuple valTuple);
    bool contains(TuplePosition pos, ValueTuple valTuple, SynonymValue val);
    bool contains(TuplePosition pos, ValueTupleSet valTupleSet, SynonymValue val);
    void addSynonym(SynonymString syn);
    void addColumnForSynonym(SynonymString syn, ValueSet vals);
    void clearGrid();
    void sortResultListBySynonym(SynonymString syn); 
    SynonymString extractSynonym(TuplePosition pos, SynonymTuple synTuple);
    GridColumn extractValue(TuplePosition pos, ValueTuple valTuple);
    GridColumn getColumnForSynonym(SynonymString syn);
    SynonymString getSynonymForColumn(GridColumn col);

public:
    ResultGrid(SynonymString syn, ValueSet vals);

    bool mergeGrid(ResultGrid* other, SynonymTuple synTuple, ValueTupleSet validTuples);
    bool updateSynonym(SynonymString syn, ValueSet vals);
    bool updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet validTuples);
    ValueSet getValuesForSynonym(SynonymString syn);
    ValueTupleSet getValuesForSynonymTuple(SynonymTuple synTuple);
};
