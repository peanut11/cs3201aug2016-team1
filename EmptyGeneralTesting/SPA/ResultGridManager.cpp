// Maintained by: Aaron
//
// Facade of:
// - ResultGrid

#include "ResultGridManager.h"

ValueTupleSet ResultGridManager::permutate(ValueSet vals1, ValueSet vals2) {
    ValueTupleSet results;

    for (ValueSet::const_iterator val1 = vals1.begin(); val1 != vals1.end(); val1++) {
        for (ValueSet::const_iterator val2 = vals2.begin(); val2 != vals2.end(); val2++) {
            results.emplace_hint(results.end(), ValueTuple(*val1, *val2));
        }
    }

    return results;
}

ResultGrid* ResultGridManager::createGridForSynonym(SynonymString syn, ValueSet vals) {
    ResultGrid* grid = new ResultGrid(syn, vals);
    refTable.push_back(grid);

    GridIndex index = gridTable.size();
    refMap[syn] = index;

    std::vector<SynonymString> synonyms;
    synonyms.push_back(syn);
    gridTable.push_back(synonyms);

    return grid;
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

void ResultGridManager::initialiseValuesForSynonym(SynonymString syn, ValueSet vals) {
    createGridForSynonym(syn, vals);
}

void ResultGridManager::updateSynonym(SynonymString syn, ValueSet vals) {
    ResultGrid* grid = getGridForSynonym(syn);
    grid->updateSynonym(syn, vals);
}

void ResultGridManager::updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet valTuples) {
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

        grid1->mergeGrid(grid2, synTuple, valTuples);

        // Update refMap and gridTable
        refMap[syn2] = index1;
        std::copy(gridTable[index2].begin(), gridTable[index2].end(), std::back_inserter(gridTable[index1]));
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
