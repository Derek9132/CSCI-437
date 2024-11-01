#include "Rodent.hpp"
#include "Cat.hpp"

/**
 * @brief Rodent constructor.
 * Calls Mobile's constructor, which calls Tile's constructor.
 *
 * @param tileId location of the rodent tile on the PNG in format rowcolumn
 * @param uniqueId unique identifier of a single rodent object
 * @param x x position on the map
 * @param y y position on the map
 * @param size size (in pixels) to render on map
 */
Rodent::Rodent(int tileId, int uniqueId, int x, int y, int size)
    : Mobile(tileId, uniqueId, x, y, size, false)
{
    // set all instance variables
    if (tileId == 136)
    {
        maxHP = 30;
        biteDamage = 1;
    }
    else if (tileId == 91)
    {
        maxHP = 16;
        biteDamage = 3;
    }
    setHealth(maxHP);
    setBiteDamage(biteDamage);

    // When values are 0, attributes are false
    setAggro(0.0);
    setStunned(0.0);
    setBiteCooldown(0.0);
    setMoveCooldown(MOVE_COOLDOWN_TIME);

    setHomePos(x, y);
    setHome(true);

    setSpeed(1.0);
}

void Rodent::resetRodent()
{
    setAggro(0);
    setBiteCooldown(0);
    setDirection(DOWN);
    setHealth(maxHP);
    setStunned(0);
}

/**
 * @brief Used to check if the owner is confined to administer the cure.
 *
 * @param map
 * @return true owner can't move
 * @return false owner can move
 */
bool Rodent::isConfined(std::shared_ptr<Map> map)
{
    // // check if there is a collision for every direction, if there isn't return false
    // if (!handleCollision(map, 0, getX() + 1, getY())) // left
    //     return false;
    // if (!handleCollision(map, 0, getX() - 1, getY())) // right
    //     return false;
    // if (!handleCollision(map, 0, getX(), getY() + 1)) // down
    //     return false;
    // if (!handleCollision(map, 0, getX(), getY() - 1)) // up
    //     return false;
    
    return true; // yay! confined!
}

/**
 * If bite is available and rodent is directly in front of cat,
 * takes away biteDamage hp from Cat.
 *
 * @param cat shared pointer to cat
 * @return true if cat died
 * @return false if cat still alive
 */
bool Rodent::bite(std::shared_ptr<Cat> cat)
{
    if (biteCooldown)
        return false;

    if (!newAnimationState(ATTACK)) // check that there's been enough time to finish the previous animation before biting
        return false;

    setBiteCooldown(BITE_COOLDOWN_TIME);
    return cat->takeDamage(biteDamage);
}

void Rodent::forceBite() {
    if (biteCooldown) 
        return;
        
    if (!newAnimationState(ATTACK)) // check that there's been enough time to finish the previous animation before biting
        return;
    return;
}

/**
 * Takes away health from the rodent and checks for death.
 *
 * @param damage amount of health to subtract
 * @return true if health drops below 0 (died)
 * @return false if still alive
 */
bool Rodent::takeDamage(int damage)
{
    int hp = getHealth();

    // If health drops below 0, handle death
    if (hp - damage <= 0)
    {
        setHealth(0);
        // TODO handleDeath() when we have that set up
        return true;
    }
    setHealth(hp - damage);
    return false;
}

/**
 * Checks if the cat is in the line of sight of the rodent using
 * Bresenham's algorithm. If so, resets isAggro timer.
 *
 * @param map shared pointer to game map
 * @param cat shared pointer to cat
 * @return true if cat is in line of sight of rodent
 * @return false if cat is not in rodent's line of sight
 */
bool Rodent::seesCat(std::shared_ptr<Map> map, std::shared_ptr<Cat> cat)
{
    int cx = cat->getX(), cy = cat->getY();
    int rx = getX(), ry = getY();

    int dx = abs(rx - cx);
    int dy = abs(ry - cy);
    int signx = (rx < cx) ? 1 : -1;
    int signy = (ry < cy) ? 1 : -1;
    int err;

    if (dx > dy)
    { // slope is x-dominant
        err = dy * 2 - dx;
        // Loop finding next tile in LOS until reach cat or a sight blocker
        do
        {
            if (err >= 0)
            {
                ry += signy;
                err -= dx * 2;
            }
            rx += signx;
            err += dy * 2;
            if (rx == cx && ry == cy)
            {
                // We've reached the cat without encountering any sight obstacles
                setAggro(AGGRO_COOLDOWN_TIME);
                return true;
            }
        } while (sightBlocked(map, rx, ry) == false);
        return false;
    }
    else
    { // slope is y-dominant
        err = dx * 2 - dy;
        do
        {
            if (err >= 0)
            {
                rx += signx;
                err -= dy * 2;
            }
            ry += signy;
            err += dx * 2;
            if (rx == cx && ry == cy)
            {
                setAggro(AGGRO_COOLDOWN_TIME);
                return true;
            }
        } while (sightBlocked(map, rx, ry) == false);
        return false;
    }
}

/**
 * Determines if there is a sight-blocking obstacle at given x, y location
 * on map. Sight-blocking tiles: wall, box, scientist.
 *
 * @param map shared pointer to game map
 * @param x x to check
 * @param y y to check
 * @return true if tile is sight-blocking
 * @return false if tile is not sight-blocking
 */
bool Rodent::sightBlocked(std::shared_ptr<Map> map, int x, int y)
{
    // Check if a wall is blocking the view
    int bgTileId = map->getBackgroundTileIdWithXYCoordinates(x, y);
    std::shared_ptr<Tile> bg = map->findTile(bgTileId);
    if (bg->getType() == WALL)
        return true;

    // Check if there is a box or scientist blocking the view
    int objTileId = map->getObjectTileIdWithXYCoordinates(x, y);
    if (!objTileId)
        return false; // No object exists at this tile
    std::shared_ptr<Tile> object = map->findTile(objTileId);
    Type type = object->getType();
    if (type == BOX || type == SCIENTIST)
    {
        return true;
    }

    return false;
}

/**
 * @brief check all four possible directions for the shortest path to the end node
 * return the direciton with the shortest path.
 * greedy graph algorithm for path finding
 *
 * @param map
 * @param goalX
 * @param goalY
 * @return true if rodent found a path
 * @return false if rodent is adjacent to and facing its goal
 */
bool Rodent::findPath(std::shared_ptr<Map> map, int goalX, int goalY, int time)
{
    std::shared_ptr<Node> currentNode = std::make_shared<Node>(getX(), getY(), getDirection());
    std::shared_ptr<Node> endNode = std::make_shared<Node>(goalX, goalY, UP); // direction isn't important for the end node
    currentNode->dist = currentNode->calcDist(endNode);
    endNode->dist = 0; // distance from end node is 0

    if (!currentNode->isAdjacent(endNode))
    {
        // check all 4 possible move locations and add to set, which will sort based on distance from end node
        // left
        if (getX() - 1 >= 0 && !handleCollision(map, time, getX() - 1, getY()))
            addNodeToSet(getX() - 1, getY(), LEFT, endNode);

        // right
        if (getX() + 1 < map->getMapX() && !handleCollision(map, time, getX() + 1, getY()))
            addNodeToSet(getX() + 1, getY(), RIGHT, endNode);

        // up
        if (getY() - 1 >= 0 && !handleCollision(map, time, getX(), getY() - 1))
            addNodeToSet(getX(), getY() - 1, UP, endNode);

        // down
        if (getY() + 1 < map->getMapY() && !handleCollision(map, time, getX(), getY() + 1))
            addNodeToSet(getX(), getY() + 1, DOWN, endNode);
    }
    else if (!isFacing(goalX, goalY))
    {
        // If adjacent, switch direction to be at object
        int x = getX(), y = getY();
        Direction dir = getDirection();
        if (goalX < x)
            setDirection(LEFT);
        else if (goalX > x)
            setDirection(RIGHT);
        else if (goalY < y)
            setDirection(UP);
        else if (goalY > y)
            setDirection(DOWN);

        return false;
    }
    else
    {
        // Rodent is adjacent to and facing goal
        return false;
    }

    std::shared_ptr<Node> minNode = currentNode;
    int minDist = currentNode->dist;

    for (auto itr : openSet)
    {
        if (itr->dist < minDist)
        {
            minNode = itr;
            minDist = itr->dist;
        }
    }
    setDirection(minNode->dir);
    minNode = NULL;
    openSet.clear();
    return true;
}

/**
 * @brief add a new node to the open set based on params
 *
 * @param x x position of the tile to move to
 * @param y y position of the tile to move to
 * @param dir direction the rodent should move to get to this tile
 * @param endNode target tile
 */
void Rodent::addNodeToSet(int x, int y, Tile::Direction dir, std::shared_ptr<Node> endNode)
{
    std::shared_ptr<Node> tryNode = std::make_shared<Node>(x, y, dir);
    tryNode->dist = tryNode->calcDist(endNode); // calculate distance from the end node
    openSet.insert(tryNode);
}

/**
 * Checks for rodent collision.
 *
 * @param map
 * @param time not used for rodents
 * @param i x-position of tile to check collision for
 * @param j y-position of tile to check collision for
 * @return true if an object or wall is in the next tile
 * @return false if no object or wall in next tile
 */
bool Rodent::handleCollision(std::shared_ptr<Map> map, int time, int i, int j)
{
    // error checking for the bounds of the map
    if (i < 0 || i == map->getMapX() || j < 0 || j == map->getMapY())
        return true; // collides with edges of the map

    int id = map->getBackgroundTileIdWithXYCoordinates(i, j); // background id
    int objId = map->getObjectTileIdWithXYCoordinates(i, j);  // object id

    if (map->findTile(id)->getType() == Tile::Type::WALL)
    {
        // wall
        return true; // collision
    }
    else if (objId == 0)
    {
        // no object
        return false;
    }
    return true; // collision
}
/**
 * Checks if the rodent is adjacent to and facing the given target
 *
 * @param targetX
 * @param targetY
 * @return true if facing target
 * @return false if not facing target
 */
bool Rodent::isFacing(int targetX, int targetY)
{
    int x = getX(), y = getY();
    Direction dir = getDirection();
    switch (dir)
    {
    case UP:
        y -= 1;
        break;
    case DOWN:
        y += 1;
        break;
    case LEFT:
        x -= 1;
        break;
    case RIGHT:
        x += 1;
        break;
    }
    if (x == targetX && y == targetY)
        return true;
    return false;
}

/**
 * Moves the rodent one tile closer to the cat.
 *
 * @param lastCatX last known x-position of cat
 * @param lastCatY last known y-position of cat
 * @param map
 * @param time
 * @return true if at cat and facing cat
 * @return false if not at cat
 */
bool Rodent::chaseCat(int lastCatX, int lastCatY, std::shared_ptr<Map> map, int time)
{
    // If at cat, return true
    int x = getX(), y = getY();
    Direction dir = getDirection();
    switch (dir)
    {
    case UP:
        y -= 1; // reminder that up is actually decreasing y values
        break;
    case DOWN:
        y += 1;
        break;
    case LEFT:
        x -= 1;
        break;
    case RIGHT:
        x += 1;
        break;
    }
    if (x == lastCatX && y == lastCatY)
        return true;

    rodentMove(map, time, true);
    return false;
}

/**
 * Moves the rodent one tile closer to its home.
 *
 * @param map
 * @param time
 * @return true if rodent is home
 * @return false if rodent is not home
 */
bool Rodent::goHome(std::shared_ptr<Map> map, int time)
{
    // If at home, set isHome = true
    if (getX() == homePos[0] && getY() == homePos[1])
        return true;

    // Else if not already moving, initiate move towards home one tile
    rodentMove(map, time, false);
    return false;
}

/**
 * @brief Acts as a wrapper for the Mobile::move method
 *
 * @param map
 * @param dir
 * @param time
 * @param cat true if following cat, false if going home
 * @return true if rodent successfully moved
 * @return false if unsuccessful in moving
 */
bool Rodent::rodentMove(std::shared_ptr<Map> map, int time, bool cat)
{
    AnimationState newState = MOVE;
    if (cat)
        newState = HISS;
    if (!newAnimationState(newState))
    {
        return false;
    }

    if (getMoveCooldown() >= 0) // check that enough time has elapsed to move again
    {
        return false;
    }
    setMoveCooldown(MOVE_COOLDOWN_TIME);

    // If moving, most likely away from home. isHome will check otherwise
    setHome(false);

    bool path;
    if (cat)
        path = findPath(map, getLastCatX(), getLastCatY(), time);
    else
    {
        path = findPath(map, getHomeX(), getHomeY(), time);
        if (!path && isFacing(getHomeX(), getHomeY()))
            move(map, getDirection(), time);
    }

    if (path)
        return move(map, getDirection(), time);
    return path;
}

bool Rodent::forceRodentMove(std::shared_ptr<Map> map, Direction dir, int time) {
    // i is future x location, j is future y location
    int i = getX();
    int j = getY();
    int tileId = map->getObjectTileIdWithXYCoordinates(i, j);

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

    // current location set to 0, which means there is no longer any object there
    map->setObjectTileIdWithXYCoordinates(getX(), getY(), 0);
    // move to a new location
    map->setObjectTileIdWithXYCoordinates(i, j, tileId);

    // set internal coordinates
    setX(i);
    setY(j);
    return true;
}

/**
 * Determines how the rodent should act based on its current state. Also
 * updates the timers for bite, aggro, and stunned.
 *
 * @param map
 * @param cat
 * @param deltaMs amount of time passed since last game loop, in milliseconds
 */
void Rodent::update(std::shared_ptr<Map> map, std::shared_ptr<Cat> cat, int deltaMs)
{
    // if (this->getTileId()
    // Convert milliseconds to seconds so we can subtract from the cooldown doubles
    //if (this->getTileId() == 131)
        //std::cout<<"owner update";

    double delta = deltaMs / 1000.0;

    //  adjust the animation duration (frame ratio) based on time elapsed if moving
    setAnimationDuration(getAnimationDuration() + delta * getSpeed());

    if (tileId == 91)
    {
        if (getAnimationDuration() >= 1) // move cooldown should be between 0 and 1. reset to 0 if over 1.
        {
            setAnimationDuration(0);
            if (isAggro())
                setAnimationState(POUNCE); // aggresive idle
            else
                setAnimationState(IDLE); // movement finished
        }
        setMoveCooldown(getMoveCooldown() - delta);
    }
    else if (tileId == 136)
    {
        if (getAnimationDuration() >= 1) // move cooldown should be between 0 and 1. reset to 0 if over 1.
        {
            setAnimationDuration(0);
            if (isAggro())
                setAnimationState(MOVE); // aggresive idle
            else
                setAnimationState(MOVE); // movement finished
        }
        setMoveCooldown(getMoveCooldown() - delta);
    }

    // Update cooldown timers
    double curStun = isStunned() - delta;
    curStun <= 0 ? setStunned(0) : setStunned(curStun);
    double curBite = getBiteCooldown() - delta;
    curBite <= 0 ? setBiteCooldown(0) : setBiteCooldown(curBite);
    double curAggro = isAggro() - delta;
    curAggro <= 0 ? setAggro(0) : setAggro(curAggro);

    if (isStunned())
        return;

    // Movement and attack logic
    if (seesCat(map, cat))
    {
        setLastCatPos(cat->getX(), cat->getY());
        // If reached cat, try and bite it
        if (chaseCat(getLastCatX(), getLastCatY(), map, deltaMs))
            bite(cat);
    }
    else
    {
        if (isAggro())
        {
            chaseCat(getLastCatX(), getLastCatY(), map, deltaMs);
        }
        else
        {
            if (!isHome())
            {
                setHome(goHome(map, deltaMs));
            }
        }
    }
}

/**void Rodent::ownerUpdate(std::shared_ptr<Map> map, std::shared_ptr<Cat> cat, int deltaMs)
{
    double delta = deltaMs / 1000.0;

    //  adjust the animation duration (frame ratio) based on time elapsed if moving
    setAnimationDuration(getAnimationDuration() + delta * getSpeed());
    if (getAnimationDuration() >= 1) // move cooldown should be between 0 and 1. reset to 0 if over 1.
    {
        setAnimationDuration(0);
        if (isAggro())
            setAnimationState(POUNCE); // aggresive idle
        else
            setAnimationState(IDLE); // movement finished
    }
    setMoveCooldown(getMoveCooldown() - delta);

    // Update cooldown timers
    double curStun = isStunned() - delta;
    curStun <= 0 ? setStunned(0) : setStunned(curStun);
    double curBite = getBiteCooldown() - delta;
    curBite <= 0 ? setBiteCooldown(0) : setBiteCooldown(curBite);
    double curAggro = isAggro() - delta;
    curAggro <= 0 ? setAggro(0) : setAggro(curAggro);

    if (isStunned())
        return;

    // Movement and attack logic
    if (seesCat(map, cat))
    {
        setLastCatPos(cat->getX(), cat->getY());
        // If reached cat, try and bite it
        if (chaseCat(getLastCatX(), getLastCatY(), map, deltaMs))
            bite(cat);
    }
    else
    {
        if (isAggro())
        {
            chaseCat(getLastCatX(), getLastCatY(), map, deltaMs);
        }
        else
        {
            if (!isHome())
            {
                setHome(goHome(map, deltaMs));
            }
        }
    }
}**/