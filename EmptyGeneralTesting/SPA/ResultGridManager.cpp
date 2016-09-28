// Maintained by: Aaron
//
// Facade of:
// - ResultGrid

#include "ResultGridManager.h"

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

ResultGrid * ResultGridManager::getGridForSynonymTuple(std::tuple<SynonymString> synTuple) {
    SynonymString syn1 = std::get<0>(synTuple);
    return getGridForSynonym(syn1);
}

ResultGridManager::GridIndex ResultGridManager::extractGridIndex(GridRefMap::const_iterator it) {
    const GridRefTuple tuple = it->second;
    const unsigned int indexPosition = 0; // colPosition = 1;
    return std::get<indexPosition>(tuple);
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

    ResultGrid* grid = getGridForSynonymTuple(synTuple);
    grid->updateSynonymTuple(synTuple, valTuples);
void ResultGridManager::updateSynonymTuple(SynonymTuple synTuple, ValueTupleSet valTuples) {
}

ValueSet ResultGridManager::getValuesForSynonym(SynonymString syn) {
    ResultGrid* grid = getGridForSynonym(syn);
    return grid->getValuesForSynonym(syn);
}

    ResultGrid* grid = getGridForSynonymTuple(synTuple);
    return grid->getValuesForSynonymTuple(synTuple);
ValueTupleSet ResultGridManager::getValuesForSynonymTuple(SynonymTuple synTuple) {
}
