// Maintained by: Aaron
//
// Accessed by:
// - ResultGridManager

#include "ResultGrid.h"

bool ResultGrid::contains(ValueSet valSet, SynonymValue val) {
    return valSet.find(val) != valSet.end();
}

bool ResultGrid::contains(ValueTupleSet valTupleSet, ValueTuple valTuple) {
    return valTupleSet.find(valTuple) != valTupleSet.end();
}

bool ResultGrid::contains(TuplePosition pos, ValueTuple valTuple, SynonymValue val) {
    return extractValue(pos, valTuple) == val;
}

bool ResultGrid::contains(TuplePosition pos, ValueTupleSet valTupleSet, SynonymValue val) {
    ValueTupleSet::const_iterator valTuple = valTupleSet.begin();
    
    while (!contains(pos, *valTuple, val) && valTuple != valTupleSet.end()) {
        valTuple++;
    }

    return valTuple == valTupleSet.end();
}

void ResultGrid::addColumnForSynonym(SynonymString syn, ValueSet vals) {
    GridColumn column = columnCount++;
    refMap[syn] = column;
    resultTable.push_back(ValueSet());

    std::list<GridRow> oldList = resultList;
    resultList = std::list<GridRow>();

    for (std::list<GridRow>::const_iterator row = oldList.begin(); row != oldList.end(); row++) {
        for (ValueSet::const_iterator val = vals.begin(); val != vals.end(); val++) {
            SynonymValue value = *val;
            resultTable[column].emplace(value);

            GridRow newRow = *row;
            newRow.push_back(value);
            resultList.push_back(newRow);
        }
    }
}

GridColumn ResultGrid::extractColumn(TuplePosition pos, SynonymTuple synTuple) {
    SynonymString syn;
    if (pos == LEFT) {
        syn = std::get<0>(synTuple);
    } else if (pos == RIGHT) {
        syn = std::get<1>(synTuple);
    } else {
        throw INVALID_TUPLE_POSITION;
    }
    return refMap[syn];
}

GridColumn ResultGrid::extractValue(TuplePosition pos, ValueTuple valTuple) {
    SynonymValue val;
    if (pos == LEFT) {
        val = std::get<0>(valTuple);
    } else if (pos == RIGHT) {
        val = std::get<1>(valTuple);
    } else {
        throw INVALID_TUPLE_POSITION;
    }
    return val;
}

ResultGrid::ResultGrid(SynonymString syn, ValueSet vals) {
    columnCount = 0;
    GridColumn column = columnCount++;
    refMap[syn] = column;
    resultTable.push_back(ValueSet());

    for (ValueSet::const_iterator val = vals.begin(); val != vals.end(); val++) {
        SynonymValue value = *val;
        resultTable[column].emplace(value);

        std::vector<SynonymValue> row;
        row.push_back(value);
        resultList.push_back(row);
    }
}

void ResultGrid::mergeGrid(ResultGrid* other, SynonymTuple synTuple, ValueTupleSet valTuples) {
    std::list<GridRow> otherList = other->resultList;

    SynonymValue value = extractValue(0, *valTuples.begin());
    SynonymValue otherValue = other->extractValue(0, *valTuples.begin());;

    for (GridListIterator row = resultList.begin(); row != resultList.end(); row++) {
        GridColumn column = extractColumn(LEFT, synTuple);
        SynonymValue value = (*row)[column];

        // TODO
    }

    // Merge and update refTable and resultTable
}

void ResultGrid::updateSynonym(SynonymString syn, ValueSet vals) {
    GridColumn column = refMap[syn];

    ValueSet intersection;
    set_intersection(resultTable[column].begin(), resultTable[column].end(), vals.begin(), vals.end(),
                     std::inserter(intersection, intersection.begin()));
    resultTable[column] = intersection;

    for (GridListIterator row = resultList.begin(); row != resultList.end(); row++) {
        SynonymValue value = (*row)[column];
        
        if (!contains(vals, value)) {
            row = resultList.erase(row);
        }
    }
}

void ResultGrid::updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet valTuples) {
    GridColumn column1 = extractColumn(LEFT, synTuple);
    GridColumn column2 = extractColumn(RIGHT, synTuple);

    for (GridListIterator row = resultList.begin(); row != resultList.end(); row++) {
        SynonymValue value1 = (*row)[column1];
        SynonymValue value2 = (*row)[column2];
        ValueTuple valueTuple = ValueTuple(value1, value2);

        if (!contains(valTuples, valueTuple)) {
            row = resultList.erase(row);
        }
    }
}

ValueSet ResultGrid::getValuesForSynonym(SynonymString syn) {
    GridColumn column = refMap[syn];
    return resultTable[column];
}

ValueTupleSet ResultGrid::getValuesForSynonymTuple(SynonymTuple synTuple) {
    GridColumn column1 = extractColumn(LEFT, synTuple);
    GridColumn column2 = extractColumn(RIGHT, synTuple);

    ValueTupleSet results;
    for (GridListIterator row = resultList.begin(); row != resultList.end(); row++) {
        SynonymValue value1 = (*row)[column1];
        SynonymValue value2 = (*row)[column2];
        ValueTuple valueTuple = ValueTuple(value1, value2);
        results.emplace_hint(results.end(), valueTuple);
    }
    return results;
}
