#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include <unordered_map>

class Map
{

private:
    std::unordered_map<int, std::shared_ptr<Tile>> tileIdtoTileMap;
    int tilePositionsToTileIds[44][32][2] = {};
    std::unordered_map<int, std::vector<std::pair<int, int>>> puzzleBoundaryCoordinates; // Key = puzzleId and Value = vector of x,y pairs

    std::shared_ptr<Tile> visibleTopLeftTile;
    int mapXDim; // number of columns
    int mapYDim; // number of rows

public:
    Map() {};

    void storeTile(int tileId, std::shared_ptr<Tile>); //store in tileIDtoTileMap
    void storePuzzleBoundaryCoordinates(int x, int y, int puzzleId);
    void setObjectTileIdWithXYCoordinates(int x, int y, int tileId); //store in tilePositionToTileIds[x][y][1]
    void setBackgroundTileIdWithXYCoordinates(int x, int y, int tileId); //store in tilePositionToTileIds[x][y][0]
    void setMapX(int x) { this->mapXDim = x; };
    void setMapY(int y) { this->mapYDim = y; };

    std::shared_ptr<Tile> findTile(int tileId) const;
    int getBackgroundTileIdWithXYCoordinates(int x, int y) const;
    int getObjectTileIdWithXYCoordinates(int x, int y) const;
    int getMapX() const { return mapXDim; };
    int getMapY() const { return mapYDim; };
    int getNumberOfPuzzles() const;
    std::vector<std::pair<int, int>> getPuzzleBoundaryCoordinates(int puzzleId) const;
};

#endif