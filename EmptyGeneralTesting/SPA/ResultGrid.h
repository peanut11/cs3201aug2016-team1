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
typedef std::vector<SynonymValue> GridRow;
typedef std::set<SynonymValue> ValueSet;
typedef std::list<GridRow>::iterator GridListIterator;
typedef GridColumnMap::const_iterator GridMapConstIter;

// Tuple of size 2
typedef std::tuple<SynonymString, SynonymString> SynonymTuple;
typedef std::tuple<SynonymValue, SynonymValue> ValueTuple;
typedef std::set<ValueTuple> ValueTupleSet;

// Tuple of variable size
typedef std::vector<GridColumn> ColumnVector;
typedef std::vector<SynonymString> SynonymVector;
typedef std::vector<SynonymValue> ValueVector;
typedef std::set<ValueVector> ValueVectorSet;

class ResultGrid {
private:
    const TuplePosition LEFT = 0;
    const TuplePosition RIGHT = 1;

    GridColumnMap refMap;              // getColumnForSynonym
    std::vector<SynonymString> refTable; // getSynonymForColumn
    std::list<GridRow> resultList;     // Possible combinations of SynonymValues
    std::vector<ValueSet> resultTable; // Unique SynonymValues for each SynonymString

    bool contains(ValueSet valSet, SynonymValue val);
    bool contains(ValueTupleSet valTupleSet, ValueTuple valTuple);
    void addSynonym(SynonymString syn);
    void addColumnForSynonym(SynonymString syn, ValueSet vals);
    void sortResultListBySynonym(SynonymString syn);
    void clearGrid();
    SynonymString extractSynonym(TuplePosition pos, SynonymTuple synTuple);
    GridColumn extractValue(TuplePosition pos, ValueTuple valTuple);
    GridColumn getColumnForSynonym(SynonymString syn);
    SynonymString getSynonymForColumn(GridColumn col);

public:
    ResultGrid(SynonymString syn, ValueSet vals);

    bool mergeGrid(ResultGrid* other, SynonymTuple synTuple, ValueTupleSet validTuples);
    bool mergeGridWithSort(ResultGrid* other, SynonymTuple synTuple, ValueTupleSet validTuples);
    bool updateSynonym(SynonymString syn, ValueSet vals);
    bool updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet validTuples);
    ValueSet getValuesForSynonym(SynonymString syn);
    ValueTupleSet getValuesForSynonymTuple(SynonymTuple synTuple);
    ValueVectorSet getValuesForSynonymTuple(SynonymVector synonyms);
};
