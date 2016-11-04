// Maintained by: Aaron
//
// Facade of:
// - ResultGrid

#include "ResultGridManager.h"

ValueTupleSet ResultGridManager::permutate(ValueSet vals1, ValueSet vals2) {
    ValueTupleSet results;

    for (ValueSet::const_iterator val1 = vals1.begin(); val1 != vals1.end(); val1++) {
        for (ValueSet::const_iterator val2 = vals2.begin(); val2 != vals2.end(); val2++) {
            results.insert(results.end(), ValueTuple(*val1, *val2));
        }
    }

    return results;
}

std::list<ValueVector> ResultGridManager::permutateForSynonymTuple(SynonymVector synonyms) {
    std::list<ValueVector> resultList = { ValueVector(synonyms.size()) };

    std::map<GridIndex, SynonymVector>  gridToSynsMap;
    std::map<GridIndex, ValueVectorSet> gridToValSetMap;
    std::map<SynonymString, GridColumn> synToColMap;

    // For each synonym, sort by grids
    for (size_t col = 0; col < synonyms.size(); col++) {
        SynonymString syn = synonyms[col];
        GridIndex id = getGridIndexForSynonym(syn);
        gridToSynsMap[id].push_back(syn);
        synToColMap[syn] = col;
    }

    // For each grid, get the values
    for (std::map<GridIndex, SynonymVector>::const_iterator it = gridToSynsMap.begin();
         it != gridToSynsMap.end(); it++) {
        GridIndex id = it->first;
        ResultGrid* grid = getGridByIndex(id);
        gridToValSetMap[id] = grid->getValuesForSynonymTuple(it->second);
    }

    // For each grid...
    for (std::map<GridIndex, ValueVectorSet>::const_iterator it = gridToValSetMap.begin();
         it != gridToValSetMap.end(); it++) {
        SynonymVector syns = gridToSynsMap[it->first];
        ValueVectorSet vecSet = it->second;

        // For each row in resultList...
        for (std::list<ValueVector>::const_iterator row = resultList.begin();
             row != resultList.end(); row = resultList.erase(row)) {

            // For each ValueVector...
            for (ValueVectorSet::const_iterator vec = vecSet.begin();
                 vec != vecSet.end(); vec++) {

                // Keep row as template
                GridRow newRow = *row;

                // Fill in each value
                for (size_t i = 0; i < vec->size(); i++) {
                    GridColumn col = synToColMap[syns[i]];
                    newRow[col] = (*vec)[i];
                }

                // Insert newRow before row
                resultList.insert(row, newRow);
            }
        }
    }

    return resultList;
}

bool ResultGridManager::createGridForSynonym(SynonymString syn, ValueSet vals) {
    ResultGrid* grid = new ResultGrid(syn, vals);
    refTable.push_back(grid);

    GridIndex index = gridTable.size();
    refMap[syn] = index;

    std::vector<SynonymString> synonyms;
    synonyms.push_back(syn);
    gridTable.push_back(synonyms);

    return refTable.size() == refMap.size() && refMap.size() == gridTable.size();
}

ResultGrid* ResultGridManager::getGridByIndex(GridIndex index) {
    return refTable[index];
}

ResultGrid* ResultGridManager::getGridForSynonym(SynonymString syn) {
    GridIndexMap::const_iterator it = refMap.find(syn);

    if (it == refMap.end()) {
        throw Exception::INVALID_SYNONYM_ERROR;
    }

    GridIndex index = it->second;
    return getGridByIndex(index);
}

GridIndex ResultGridManager::getGridIndexForSynonym(SynonymString syn) {
    GridIndexMap::const_iterator it = refMap.find(syn);
    return it->second;
}

bool ResultGridManager::areInSameGrid(SynonymString syn1, SynonymString syn2) {
    return getGridIndexForSynonym(syn1) == getGridIndexForSynonym(syn2);
}

bool ResultGridManager::areInSameGrid(SynonymVector synonyms) {
    const GridIndex id = getGridIndexForSynonym(synonyms[0]);

    for (size_t i = 1; i < synonyms.size(); i++) {
        if (id != getGridIndexForSynonym(synonyms[i])) {
            return false;
        }
    }

    return true;
}

bool ResultGridManager::initialiseSynonym(SynonymString syn, ValueSet vals) {
    return createGridForSynonym(syn, vals);
}

bool ResultGridManager::updateSynonym(SynonymString syn, ValueSet vals) {
    ResultGrid* grid = getGridForSynonym(syn);
    return grid->updateSynonym(syn, vals);
}

bool ResultGridManager::updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet valTuples) {
    SynonymString syn1 = std::get<0>(synTuple);
    SynonymString syn2 = std::get<1>(synTuple);

    if (areInSameGrid(syn1, syn2)) {
        ResultGrid* grid = getGridForSynonym(syn1);
        return grid->updateSynonymTuple(synTuple, valTuples);

    } else {
        GridIndex index1 = getGridIndexForSynonym(syn1);
        GridIndex index2 = getGridIndexForSynonym(syn2); 
        ResultGrid* grid1 = getGridByIndex(index1);
        ResultGrid* grid2 = getGridByIndex(index2);

        // Update refMap and gridTable
        for (std::vector<SynonymString>::const_iterator syn = gridTable[index2].begin();
             syn != gridTable[index2].end(); syn++) {
            refMap[*syn] = index1;
            gridTable[index1].push_back(*syn);
        }

        return grid1->mergeGrid(grid2, synTuple, valTuples);
    }
}

ValueSet ResultGridManager::getValuesForSynonym(SynonymString syn) {
    ResultGrid* grid = getGridForSynonym(syn);
    return grid->getValuesForSynonym(syn);
}

ValueTupleSet ResultGridManager::getValuesForSynonymTuple(SynonymTuple synTuple) {
    SynonymString syn1 = std::get<0>(synTuple);
    SynonymString syn2 = std::get<1>(synTuple);

    if (areInSameGrid(syn1, syn2)) {
        ResultGrid* grid = getGridForSynonym(syn1);
        return grid->getValuesForSynonymTuple(synTuple);

    } else {
        ValueSet valSet1 = getValuesForSynonym(syn1);
        ValueSet valSet2 = getValuesForSynonym(syn2);
        return permutate(valSet1, valSet2);
    }
    
    return ValueTupleSet();
}

ValueVectorSet ResultGridManager::getValuesForSynonymTuple(SynonymVector synonyms) {
    ValueVectorSet result;

    if (areInSameGrid(synonyms)) {
        ResultGrid* grid = getGridForSynonym(synonyms[0]);
        result = grid->getValuesForSynonymTuple(synonyms);
    } else {
        std::list<ValueVector> resultList = permutateForSynonymTuple(synonyms);
        result = ValueVectorSet(resultList.begin(), resultList.end());
    }

    return result;
}
