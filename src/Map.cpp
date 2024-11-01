#include "Map.hpp"
#include <iostream>

void Map::storeTile(int tileId, std::shared_ptr<Tile> tile)
{
    tileIdtoTileMap.insert({tileId, tile});
}

void Map::setBackgroundTileIdWithXYCoordinates(int x, int y, int tileId)
{
    tilePositionsToTileIds[x][y][0] = tileId;
}

void Map::setObjectTileIdWithXYCoordinates(int x, int y, int tileId)
{
    tilePositionsToTileIds[x][y][1] = tileId;
}

void Map::storePuzzleBoundaryCoordinates(int x, int y, int puzzleId)
{
    std::vector<std::pair<int, int>> coordinatesVector;
    std::pair<int, int> coordinates;
    coordinates = std::make_pair(x, y);

    // Check if puzzle id is a hashmap key already
    if (puzzleBoundaryCoordinates.count(puzzleId)) {
        // Store in existing vector
        coordinatesVector = puzzleBoundaryCoordinates.at(puzzleId);
    }

    coordinatesVector.push_back(coordinates);
    puzzleBoundaryCoordinates[puzzleId] = coordinatesVector;
}

std::shared_ptr<Tile> Map::findTile(int tileId) const
{
    return tileIdtoTileMap.at(tileId);
}

int Map::getBackgroundTileIdWithXYCoordinates(int x, int y) const
{
    return tilePositionsToTileIds[x][y][0];
}

int Map::getObjectTileIdWithXYCoordinates(int x, int y) const
{
    return tilePositionsToTileIds[x][y][1];
}

std::vector<std::pair<int, int>> Map::getPuzzleBoundaryCoordinates(int puzzleId) const
{
    return puzzleBoundaryCoordinates.at(puzzleId);
}

int Map::getNumberOfPuzzles() const 
{
    return puzzleBoundaryCoordinates.size();
}