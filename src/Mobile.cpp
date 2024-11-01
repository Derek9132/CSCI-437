#include "Mobile.hpp"


Mobile::Mobile(int tileId, int uniqueId, int x, int y, int size, bool isBox)
    : Tile(tileId, x, y, size), isBox(isBox), uniqueId(uniqueId)
{
    speed = 0.0;
    animationDuration = 0.0;
}

/**
 * @brief checks for collision in next location of mobile object
 *
 * @param map
 * @param i x coordinate of next location
 * @param j y coordinate of next location
 * @return true collision will occur
 * @return false no collision will occur
 */
bool Mobile::handleCollision(std::shared_ptr<Map> map, int time, int i, int j)
{
    // handle map edge cases
    if (i < 0 || i >= map->getMapX() || j < 0 || j >= map->getMapY())
        return true;
    int id = map->getBackgroundTileIdWithXYCoordinates(i, j); // background id
    int objId = map->getObjectTileIdWithXYCoordinates(i, j);  // object id

    // if object ID is 0, there is no object present
    // if background ID maps to a tile with type wall, that causes a collision
    if (objId == 0 && map->findTile(id)->getType() != Tile::Type::WALL)
        return false; // no collision

    return true;
}

/**
 * @brief move a mobile object one space in a given direction
 *
 * @param map map object that acts as a data structure
 * @param dir up, down, left, or right
 * @param time change in milliseconds
 * @return true object moved
 * @return false object did not move
 */
bool Mobile::move(std::shared_ptr<Map> map, Direction dir, int time)
{
    int idSave;

    // i is future x location, j is future y location
    int i = getX();
    int j = getY();

    idSave = map->getObjectTileIdWithXYCoordinates(i, j);
    //animationState = MOVE;

    switch (dir)
    {
    case UP:
        j -= 1; // reminder that up is actually decreasing y values
        setDirection(UP);
        break;
    case DOWN:
        j += 1;
        setDirection(DOWN);
        break;
    case LEFT:
        i -= 1;
        setDirection(LEFT);
        break;
    case RIGHT:
        i += 1;
        setDirection(RIGHT);
        break;
    }

    // check boundaries of the map because objects cannot exceed map boundaries
    if (i < 0 || i > map->getMapX() - 1 || j < 0 || j > map->getMapY() - 1)
        return false;

    // in the case of cat, handleCollision also handles box pushing and returns whether the cat can move
    //      (false = no collision aka cat move or true collision and cat can't move).
    // in all other casess, handleCollision returns whether a collision will occur
    if (handleCollision(map, time, i, j))
        return false;

    // current location set to 0, which means there is no longer any object there
    map->setObjectTileIdWithXYCoordinates(getX(), getY(), 0);
    // move to a new location
    map->setObjectTileIdWithXYCoordinates(i, j, idSave);

    // set internal coordinates
    setX(i);
    setY(j);
    return true;
}

/**
 * @brief Reset the animation ratio (animationDuration) and change the animationState
 * based on the current state
 *
 * @param state new animation state
 * @return true animation state changed
 * @return false animation state did not change
 */
bool Mobile::newAnimationState(AnimationState state)
{   
    // in order to change the animation state, the input state has to be new
    // otherwise it's the middle of an animation and the animation state should not be changed
    if (state != animationState)
    {
        animationDuration = 0.0;
        setAnimationState(state);
        return true;
    }
    return false;
}