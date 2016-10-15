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

void ResultGrid::sortResultListBySynonym(SynonymString syn) {
    GridColumn column = getColumnForSynonym(syn);
    std::stable_sort(resultList.begin(), resultList.end(), [column](GridRow row1, GridRow row2) {
        return (row1[column] < row2[column]); });
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
    return mergeGridBruteForce(other, synTuple, validTuples);

    /*
    // If no valid tuples, then clear grid and return
    if (validTuples.empty()) {
        clearGrid();
        other->clearGrid();
        return false;
    }

    // Transfer empty columns
    for (size_t otherCol = 0; otherCol < other->refTable.size(); otherCol++) {
        SynonymString otherSyn = other->refTable[otherCol];
        addSynonym(otherSyn);
    }

    // Sort resultList in both grids by the synonyms of interest
    SynonymString syn = extractSynonym(LEFT, synTuple);
    SynonymString otherSyn = extractSynonym(RIGHT, synTuple);
    sortResultListBySynonym(syn);
    other->sortResultListBySynonym(otherSyn);

    // Get ready to merge
    GridColumn column = getColumnForSynonym(syn);
    GridColumn otherColumn = other->getColumnForSynonym(otherSyn);
    ValueTupleSet::const_iterator validTuple = validTuples.begin();

    // Prepare to store updated synonym values
    ValueSet synSet;
    ValueSet otherSynSet;

    // Loop through resultList
    for (GridListIterator row = resultList.begin(); row != resultList.end(); row = resultList.erase(row)) {

        // Go to a valid row in resultList
        SynonymValue validValue = extractValue(LEFT, *validTuple);
        while (row != resultList.end() && (((*row)[column]) != validValue)) {
            row = resultList.erase(row);
        }

        // If at end of resultList, stop looping
        if (row == resultList.end()) {
            break;
        }

        // Continue down the validTuples while the left side is the same as currentValue
        while (validTuple != validTuples.end() && extractValue(LEFT, *validTuple) == validValue) {

            // Go to a valid otherRow in other->ResultList
            GridListIterator otherRow = other->resultList.begin();
            SynonymValue otherValidValue = extractValue(RIGHT, *validTuple);
            while (otherRow != other->resultList.end() && (*otherRow)[otherColumn] != otherValidValue) {
                otherRow++;
            }

            // If at end of other->resultList, stop looping
            if (otherRow == other->resultList.end()) {
                break;
            }
            
            throw std::runtime_error("");

            // Permutate
            while (otherRow != other->resultList.end() && (*otherRow)[otherColumn] == otherValidValue) {
                throw std::runtime_error("");
                // Add synonym to updated synonym values
                synSet.insert(validValue);
                otherSynSet.insert(otherValidValue);

                // Keep row as template
                GridRow newRow = *row;
                
                // Contatenate otherRow to newRow
                std::copy((*otherRow).begin(), (*otherRow).end(), std::back_inserter(newRow));
                
                // Insert newRow before row
                resultList.insert(row, newRow);

                // Proceed to next otherRow
                otherRow++;
            }

            // Proceed to next validTuple
            validTuple++;
        }
    }

    // Replace previous valid values with updated synonym values
    resultTable[column] = synSet;
    otherColumn = getColumnForSynonym(otherSyn);
    resultTable[otherColumn] = otherSynSet;

    return !resultList.empty();
    */
}

bool ResultGrid::mergeGridBruteForce(ResultGrid * other, SynonymTuple synTuple, ValueTupleSet validTuples) {
    // If no valid tuples, then clear grid and return
    if (validTuples.empty()) {
        clearGrid();
        other->clearGrid();
        return false;
    }

    // Permutate
    for (GridListIterator row = resultList.begin(); row != resultList.end(); row = resultList.erase(row)) {
        for (GridListIterator otherRow = other->resultList.begin(); otherRow != other->resultList.end(); otherRow++) {
            // Keep row as template
            size_t prevRowSize = (*row).size();
            GridRow newRow = *row;

            // Contatenate otherRow to newRow
            std::copy((*otherRow).begin(), (*otherRow).end(), std::back_inserter(newRow));

            // Check that row was not modified
            if (prevRowSize != (*row).size()) {
                throw std::runtime_error("");
            }

            // Insert newRow before row
            size_t prevResultListSize = resultList.size();
            resultList.insert(row, newRow);

            // Check that newRow was inserted
            if (prevResultListSize + 1 != resultList.size()) {
                throw std::runtime_error("");
            }
        }
    }

    // Transfer empty columns
    for (size_t otherColumn = 0; otherColumn < other->refTable.size(); otherColumn++) {
        SynonymString otherSyn = other->refTable[otherColumn];
        addSynonym(otherSyn);
    }

    // Prepare to store updated synonym values
    std::vector<ValueSet> newResultTable;
    while (newResultTable.size() < resultTable.size()) {
        newResultTable.push_back(ValueSet());
    }

    SynonymString syn = extractSynonym(LEFT, synTuple);
    SynonymString otherSyn = extractSynonym(RIGHT, synTuple);
    GridColumn column = getColumnForSynonym(syn);
    GridColumn otherColumn = getColumnForSynonym(otherSyn);

    for (GridListIterator row = resultList.begin(); row != resultList.end(); /* updated in loop */) {
        bool isValidRow = false;

        SynonymValue synVal = (*row)[column];
        SynonymValue otherSynVal = (*row)[otherColumn];
        ValueTupleSet::const_iterator validT = validTuples.begin();

        while (!isValidRow && validT != validTuples.end()) {
            SynonymValue validSynVal = extractValue(LEFT, *validT);
            SynonymValue otherValidSynVal = extractValue(RIGHT, *validT);

            if ((synVal == validSynVal) && (otherSynVal == otherValidSynVal)) {
                isValidRow = true;
            } else {
                validT++;
            }
        }

        if (isValidRow) {
            // Update newResultTable
            for (size_t column = 0; column < newResultTable.size(); column++) {
                newResultTable[column].insert((*row)[column]);
            }

            row++;
        } else {
            row = resultList.erase(row);
        }
    }

    resultTable = newResultTable;
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

    GridListIterator row = resultList.begin();
    while (row != resultList.end()) {
        SynonymValue value = (*row)[column];

        if (!contains(intersection, value)) {
            row = resultList.erase(row);
        } else {
            row++;
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

    for (GridListIterator row = resultList.begin(); row != resultList.end(); row++) {
        SynonymValue value1 = (*row)[column1];
        SynonymValue value2 = (*row)[column2];
        ValueTuple valueTuple = ValueTuple(value1, value2);

        if (!contains(validTuples, valueTuple)) {
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
