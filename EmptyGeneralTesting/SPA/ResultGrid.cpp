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

void ResultGrid::addSynonym(SynonymString syn) {
    refMap[syn] = refTable.size();
    refTable.push_back(syn);
    resultTable.push_back(ValueSet());
}

void ResultGrid::addColumnForSynonym(SynonymString syn, ValueSet vals) {
    addSynonym(syn);
    GridColumn column = getColumnForSynonym(syn);

    std::list<GridRow> oldList = resultList;
    resultList = std::list<GridRow>();

    for (std::list<GridRow>::const_iterator row = oldList.begin(); row != oldList.end(); row++) {
        for (ValueSet::const_iterator val = vals.begin(); val != vals.end(); val++) {
            SynonymValue value = *val;
            resultTable[column].insert(value);

            GridRow newRow = *row;
            newRow.push_back(value);
            resultList.push_back(newRow);
        }
    }
}

void ResultGrid::clearGrid() {
    resultList.clear();
    resultTable.clear();
}

SynonymString ResultGrid::extractSynonym(TuplePosition pos, SynonymTuple synTuple) {
    SynonymString syn;
    if (pos == LEFT) {
        syn = std::get<0>(synTuple);
    } else if (pos == RIGHT) {
        syn = std::get<1>(synTuple);
    } else {
        throw Exception::INVALID_TUPLE_POSITION;
    }
    return syn;
}

GridColumn ResultGrid::extractValue(TuplePosition pos, ValueTuple valTuple) {
    SynonymValue val;
    if (pos == LEFT) {
        val = std::get<0>(valTuple);
    } else if (pos == RIGHT) {
        val = std::get<1>(valTuple);
    } else {
        throw Exception::INVALID_TUPLE_POSITION;
    }
    return val;
}

GridColumn ResultGrid::getColumnForSynonym(SynonymString syn) {
    return refMap[syn];
}

SynonymString ResultGrid::getSynonymForColumn(GridColumn col) {
    return refTable[col];
}

ResultGrid::ResultGrid(SynonymString syn, ValueSet vals) {
    addSynonym(syn);
    GridColumn column = getColumnForSynonym(syn);

    for (ValueSet::const_iterator val = vals.begin(); val != vals.end(); val++) {
        SynonymValue value = *val;
        resultTable[column].insert(value);

        std::vector<SynonymValue> row;
        row.push_back(value);
        resultList.push_back(row);
    }
}

bool ResultGrid::mergeGrid(ResultGrid* other, SynonymTuple synTuple, ValueTupleSet validTuples) {
    // If no valid tuples, then clear grid and return
    if (validTuples.empty()) {
        clearGrid();
        other->clearGrid();
        return false;
    }

    // Clear previous synonym values
    for (std::vector<ValueSet>::iterator synVal = resultTable.begin(); synVal != resultTable.end(); synVal++) {
        synVal->clear();
    }

    // Transfer empty columns
    for (size_t otherCol = 0; otherCol < other->refTable.size(); otherCol++) {
        SynonymString otherSyn = other->refTable[otherCol];
        addSynonym(otherSyn);
    }

    // Optimisation for 2 grids that contain 1 synonym each
    if (refTable.size() == 2 && other->refTable.size() == 1) {
        resultList.clear();

        for (ValueTupleSet::const_iterator it = validTuples.begin(); it != validTuples.end(); it++) {
            // Create newRow
            GridRow newRow = { extractValue(LEFT, *it), extractValue(RIGHT, *it) };
            // Insert newRow
            resultList.push_back(newRow);
            // Insert synonym values into resultTable
            for (size_t column = 0; column < resultTable.size(); column++) {
                resultTable[column].insert((newRow)[column]);
            }
        }

        return true;
    }

    GridColumn column = getColumnForSynonym(extractSynonym(LEFT, synTuple));
    GridColumn otherColumn = other->getColumnForSynonym(extractSynonym(RIGHT, synTuple));

    std::set<SynonymValue> synSet;
    std::set<SynonymValue> otherSynSet;

    for (ValueTupleSet::const_iterator it = validTuples.begin(); it != validTuples.end(); it++) {
        synSet.insert(extractValue(LEFT, *it));
        otherSynSet.insert(extractValue(RIGHT, *it));
    }

    // Permutate
    for (GridListIterator row = resultList.begin(); row != resultList.end(); row = resultList.erase(row)) {
        SynonymValue synVal = (*row)[column];

        if (!contains(synSet, synVal)) {
            continue;
        }

        GridListIterator otherRow = other->resultList.begin();
        while (otherRow != other->resultList.end()) {
            SynonymValue otherSynVal = (*otherRow)[otherColumn];

            if (!contains(otherSynSet, otherSynVal)) {
                otherRow = other->resultList.erase(otherRow);
                continue;
            }

            ValueTuple valueTuple = ValueTuple(synVal, otherSynVal);
            // Only permutate valid tuples
            if (contains(validTuples, valueTuple)) {
                // Keep row as template
                GridRow newRow = *row;
                // Contatenate otherRow to newRow
                std::copy((*otherRow).begin(), (*otherRow).end(), std::back_inserter(newRow));
                // Insert newRow before row
                resultList.insert(row, newRow);
                // Insert synonym values into resultTable
                for (size_t column = 0; column < resultTable.size(); column++) {
                    resultTable[column].insert((newRow)[column]);
                }
            }

            otherRow++;
        }
    }

    return !resultList.empty();
}

bool ResultGrid::updateSynonym(SynonymString syn, ValueSet vals) {
    if (vals.empty()) {
        clearGrid();
        return false;
    }

    GridColumn column = refMap[syn];

    ValueSet intersection;
    set_intersection(resultTable[column].begin(), resultTable[column].end(), vals.begin(), vals.end(),
                     std::inserter(intersection, intersection.begin()));
    resultTable[column] = intersection;

    if (intersection.empty()) {
        clearGrid();
        return false;
    }

    // Clear previous synonym values
    for (std::vector<ValueSet>::iterator synVal = resultTable.begin(); synVal != resultTable.end(); synVal++) {
        synVal->clear();
    }

    GridListIterator row = resultList.begin();
    while (row != resultList.end()) {
        SynonymValue value = (*row)[column];

        if (contains(intersection, value)) {
            // Insert valid synonym values
            for (size_t column = 0; column < resultTable.size(); column++) {
                resultTable[column].insert((*row)[column]);
            }

            row++;
        } else {
            row = resultList.erase(row);
        }
    }

    return true;
}

bool ResultGrid::updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet validTuples) {
    if (validTuples.empty()) {
        clearGrid();
        return false;
    }

    GridColumn column1 = getColumnForSynonym(extractSynonym(LEFT, synTuple));
    GridColumn column2 = getColumnForSynonym(extractSynonym(RIGHT, synTuple));

    // Clear previous synonym values
    for (std::vector<ValueSet>::iterator synVal = resultTable.begin(); synVal != resultTable.end(); synVal++) {
        synVal->clear();
    }
    
    GridListIterator row = resultList.begin();

    while (row != resultList.end()) {
        SynonymValue value1 = (*row)[column1];
        SynonymValue value2 = (*row)[column2];
        ValueTuple valueTuple = ValueTuple(value1, value2);

        if (contains(validTuples, valueTuple)) {
            // Insert valid synonym values
            for (size_t column = 0; column < resultTable.size(); column++) {
                resultTable[column].insert((*row)[column]);
            }

            row++;
        } else {
            row = resultList.erase(row);
        }
    }

    return !resultList.empty();
}

ValueSet ResultGrid::getValuesForSynonym(SynonymString syn) {
    GridColumn column = refMap[syn];
    return resultTable[column];
}

ValueTupleSet ResultGrid::getValuesForSynonymTuple(SynonymTuple synTuple) {
    GridColumn column1 = getColumnForSynonym(extractSynonym(LEFT, synTuple));
    GridColumn column2 = getColumnForSynonym(extractSynonym(RIGHT, synTuple));

    ValueTupleSet results;
    for (GridListIterator row = resultList.begin(); row != resultList.end(); row++) {
        SynonymValue value1 = (*row)[column1];
        SynonymValue value2 = (*row)[column2];
        ValueTuple valueTuple = ValueTuple(value1, value2);
        results.insert(results.end(), valueTuple);
    }
    return results;
}
