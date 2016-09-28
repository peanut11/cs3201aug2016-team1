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

ResultGrid* ResultGridManager::getGridForSynonym(SynonymString syn) {
    GridIndexMap::const_iterator it = refMap.find(syn);
    ResultGrid* grid;

    if (it == refMap.end()) {
        throw INVALID_SYNONYM_ERROR;
    } else {
        GridIndex index = it->second;
        grid = refTable[index];
    }

    return grid;
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
        ResultGrid* grid1 = getGridForSynonym(syn1);
        ResultGrid* grid2 = getGridForSynonym(syn2);

        grid1->mergeGrid(grid2, synTuple, valTuples);
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
