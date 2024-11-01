#ifndef IMMOBILE_HPP
#define IMMOBILE_HPP
#include "Tile.hpp"
#include "Map.hpp"

class Immobile : public Tile
{
private:
    int id;     // Used by Items to update game state

public:
    Immobile(int tileId, int x, int y, int size, int id)
    : Tile(tileId, x, y, size) { this->id = id; };

    int getId() { return id; };

};

#endif