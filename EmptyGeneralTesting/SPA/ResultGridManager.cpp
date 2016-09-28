// Maintained by: Aaron
//
// Facade of:
// - ResultGrid

#include "ResultGridManager.h"

ResultGrid* ResultGridManager::createGridForSynonym(SynonymString syn, std::set<SynonymValue> vals) {
    ResultGrid* grid = new ResultGrid(syn, vals);
    refTable.push_back(grid);

    GridIndex index = gridTable.size();
    GridColumn column = 0;
    refMap[syn] = GridRefTuple(index, column);

    std::vector<SynonymString> synonyms;
    synonyms.push_back(syn);
    gridTable.push_back(synonyms);

    return grid;
}

ResultGrid* ResultGridManager::getGridForSynonym(SynonymString syn) {
    GridRefMap::const_iterator it = refMap.find(syn);
    ResultGrid* grid;

    if (it == refMap.end()) {
        throw INVALID_SYNONYM_ERROR;
    } else {
        grid = refTable[extractGridIndex(it)];
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

ResultGridManager::GridIndex ResultGridManager::getGridIndexForSynonym(SynonymString syn) {
    GridRefMap::const_iterator it = refMap.find(syn);
    return extractGridIndex(it);
}

bool ResultGridManager::areInSameGrid(SynonymString syn1, SynonymString syn2) {
    return getGridIndexForSynonym(syn1) == getGridIndexForSynonym(syn2);
}

void ResultGridManager::initialiseValuesForSynonym(SynonymString syn, std::set<SynonymValue> vals) {
    createGridForSynonym(syn, vals);
}

void ResultGridManager::updateSynonym(SynonymString syn, std::set<SynonymValue> vals) {
    ResultGrid* grid = getGridForSynonym(syn);
    grid->updateSynonym(syn, vals);
}

void ResultGridManager::updateSynonymTuple(std::tuple<SynonymString> synTuple, std::set<std::tuple<SynonymValue>> valTuples) {
    ResultGrid* grid = getGridForSynonymTuple(synTuple);
    grid->updateSynonymTuple(synTuple, valTuples);
}

std::set<SynonymValue> ResultGridManager::getValuesForSynonym(SynonymString syn) {
    ResultGrid* grid = getGridForSynonym(syn);
    return grid->getValuesForSynonym(syn);
}

std::set<std::tuple<SynonymValue>> ResultGridManager::getValuesForSynonymTuple(std::tuple<SynonymString> synTuple) {
    ResultGrid* grid = getGridForSynonymTuple(synTuple);
    return grid->getValuesForSynonymTuple(synTuple);
}
