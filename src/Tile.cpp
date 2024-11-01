#include "Tile.hpp"

Tile::Tile(int tileId, int x, int y, int size, Direction direction)
    : tileId(tileId), x(x), y(y), size(size), direction(direction)
{
    type = tileIdToType(tileId);
    animationState = IDLE;
}

/**
 * @brief Takes the Tile ID (which is based on the tile's location on the PNG)
 * and translates it to the object type. Objects of same type that use different 
 * pngs use direction to differentiate between pngs for the AnimationManager.
 *
 * @param tileId based on the ID from the Tiled Map Editor
 * @return enum Tile::Type
 */
enum Tile::Type Tile::tileIdToType(int tileId)
{
    if (tileId > 0 && tileId < 16 || tileId == 21 || tileId == 22)
    {
        return Tile::Type::BACKGROUND;
    }
    if (tileId > 15 && tileId < 21 || tileId == 23)
    {
        return Tile::Type::WALL;
    }

    switch (tileId)
    {
    case 31:
        return Tile::Type::BOX;
    case 41:
    {
        // item 1: vial
        direction = UP;
        return Tile::Type::ITEM;
    }
    case 42:
    {
        // item 2: thermometer
        direction = DOWN;
        return Tile::Type::ITEM;
    }
    case 43:
    {
        // item 3: plant
        direction = LEFT;
        return Tile::Type::ITEM;
    }
    case 44:
    {
        // item 4: beaker
        direction = RIGHT;
        return Tile::Type::ITEM;
    }
    case 51:
    {
        // scientist 4: Derek
        direction = RIGHT;
        return Tile::Type::SCIENTIST;
    }
    case 61:
    {
        // scientist 1: Seb
        direction = UP;
        return Tile::Type::SCIENTIST;
    }
    case 71:
    {
        // scientist 3: Colleen
        direction = LEFT;
        return Tile::Type::SCIENTIST;
    }
    case 81:
    {
        // scientist 2: Janelle
        direction = DOWN;
        return Tile::Type::SCIENTIST;
    }
    case 91:
        return Tile::Type::RODENT;
    case 131: // owner
        return Tile::Type::OWNER;
    case 171:
        return Tile::Type::CAT;
    }
    return Tile::Type::BACKGROUND;
}