#pragma once

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

class ResultGrid {
private:
    const TuplePosition LEFT = 0;
    const TuplePosition RIGHT = 0;

    GridColumn columnCount;
    GridColumnMap refMap;              // getColumnForSynonym
    std::list<GridRow> resultList;     // Possible combinations of SynonymValues
    std::vector<ValueSet> resultTable; // Unique SynonymValues for each SynonymString

    bool contains(ValueSet valSet, SynonymValue val);
    bool contains(ValueTupleSet valTupleSet, ValueTuple valTuple);
    bool contains(TuplePosition pos, ValueTuple valTuple, SynonymValue val);
    bool contains(TuplePosition pos, ValueTupleSet valTupleSet, SynonymValue val);
    void addColumnForSynonym(SynonymString syn, ValueSet vals);
    GridColumn extractColumn(TuplePosition pos, SynonymTuple synTuple);
    GridColumn extractValue(TuplePosition pos, ValueTuple valTuple);

public:
    ResultGrid(SynonymString syn, ValueSet vals);

    void mergeGrid(ResultGrid* other, SynonymTuple synTuple, ValueTupleSet valTuples);
    void updateSynonym(SynonymString syn, ValueSet vals);
    void updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet valTuples);
    ValueSet getValuesForSynonym(SynonymString syn);
    ValueTupleSet getValuesForSynonymTuple(SynonymTuple synTuple);
};
