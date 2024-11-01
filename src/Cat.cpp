#include "Cat.hpp"
#include "Rodent.hpp"

/**
 * @brief Cat constructor.
 * Calls Mobile's constructor, which calls Tile's constructor.
 *
 * @param tileId location of the cat tile on the PNG in format rowcolumn
 * @param uniqueId unique identifier of the cat object
 * @param x x position on the map
 * @param y y position on the map
 * @param size size (in pixels) to render on map
 */
Cat::Cat(int tileId, int uniqueId, int x, int y, int size)
    : Mobile(tileId, uniqueId, x, y, size, false)
{
    // assign values to all instance variables
    health = MAX_HP;
    scratchCooldown = 0.0;
    scratchAmmo = 3;
    hissCooldown = 0.0;

    // same as animation speed
    setSpeed(4.0);
}

void Cat::resetCat()
{
    setDirection(DOWN);
    setHealth(MAX_HP);
    setHissCooldown(0);
    setScratchAmmo(3);
    setScratchCooldown(0);
}

/**
 * Returns the Rodent directly in front of the cat, if it exists.
 * If no object is there or is unattackable (Scientist, etc), returns none.
 * @param map the game map data structure
 * @param dir direction the cat is facing
 * @param i the current x position of the cat
 * @param j the current y position of the cat
 * @return shared pointer to rodent to attack *
 **/
std::shared_ptr<Rodent> Cat::getTarget(std::shared_ptr<Map> map, Direction dir, int i, int j)
{

    int ti = i;
    int tj = j;

    switch (dir)
    {
    case Direction::UP:
        tj -= 1;
        break;
    case Direction::DOWN:
        tj += 1;
        break;
    case Direction::LEFT:
        ti -= 1;
        break;
    case Direction::RIGHT:
        ti += 1;
        break;
    }

    int objId = map->getObjectTileIdWithXYCoordinates(ti, tj);

    if (objId == 0)
    {
        return nullptr;
    }
    else
    {
        Type type = map->findTile(objId)->getType();
        if (type == RODENT || type == OWNER)
        {
            std::shared_ptr<Tile> rodentTile = map->findTile(objId);
            std::shared_ptr<Rodent> target = std::dynamic_pointer_cast<Rodent>(rodentTile);
            return target;
        }
        return nullptr;
    }
}

/**
 * If scratch is available, takes away 4hp from enemy immediately in front of cat and adds time to scratchCooldown.
 *
 * @param map the game map data structure
 * @param deltaMs number of milliseconds passed since last game loop
 * @return true if enemy is killed, false if still alive
 */
bool Cat::scratch(std::shared_ptr<Map> map, int deltaMs)
{
    if (scratchCooldown > 6.0)
    {
        return false;
    }
    // check if enough time has elapsed for a new scratch animation
    if (!newAnimationState(ATTACK))
    {
        return false;
    }
    setScratchCooldown(scratchCooldown + SCRATCH_COOLDOWN_TIME);

    std::shared_ptr<Rodent> target = getTarget(map, Mobile::getDirection(), getX(), getY());

    if (!target)
    {
        return false;
    }

    bool killed = target->takeDamage(SCRATCH_DAMAGE);
    if (killed)
    {
        map->setObjectTileIdWithXYCoordinates(target->getX(), target->getY(), 0);
    }
    return killed;
}

/**
 * Forces a scratch. Used for cutscenes
 * 
 */
void Cat::forceScratch()
{
    if (scratchCooldown > 6.0)
    {
        return;
    }
    // check if enough time has elapsed for a new scratch animation
    if (!newAnimationState(ATTACK))
    {
        return;
    }
    setScratchCooldown(scratchCooldown + SCRATCH_COOLDOWN_TIME);

    return;
}


/**
 * Forces a pet. Used for cutscenes
 * 
 */
void Cat::forcePet()
{
    if (!newAnimationState(PET))
    {
        return;
    }
}


/**
 * If hiss is available, stuns all enemies in a 4 tile radius and resets the hissCooldown timer.
 *
 * @param map the game map data structure
 * @param deltaMs number of milliseconds passed since last game loop
 */
void Cat::hiss(std::shared_ptr<Map> map, int deltaMs)
{
    if (hissCooldown)
        return;
    // check if enough time has passed for a new animation state
    if (!newAnimationState(HISS))
    {
        return;
    }

    setHissCooldown(HISS_COOLDOWN_TIME);

    // Identify enemies within 4 tiles
    std::vector<std::shared_ptr<Rodent>> targets;
    int catX = getX(), catY = getY();

    int x = catX - HISS_RAD, endX = catX + HISS_RAD;
    if (x < 0)
        x = 0;
    if (endX > map->getMapX())
        endX = map->getMapX();

    int startY = catY - HISS_RAD, endY = catY + HISS_RAD;
    if (startY < 0)
        startY = 0;
    if (endY > map->getMapY())
        endY = map->getMapY();

    for (; x <= endX; x++)
    {
        for (int y = startY; y <= endY; y++)
        {
            int objId = map->getObjectTileIdWithXYCoordinates(x, y);
            if (objId == 0)
                continue; // no object there, check next tile

            Type type = map->findTile(objId)->getType();
            if (type == RODENT || type == OWNER)
            {
                std::shared_ptr<Tile> rodentTile = map->findTile(objId);
                std::shared_ptr<Rodent> target = std::dynamic_pointer_cast<Rodent>(rodentTile);
                if (target)
                    // Render rodent immobile for 4 seconds
                    target->setStunned(HISS_STUN_TIME);
            }
        }
    }

    // Render them immobile for 4 seconds
    for (std::shared_ptr<Rodent> target : targets)
    {
        target->setStunned(HISS_STUN_TIME);
    }
}

/**
 * Increments health of cat.
 *
 * @param hp amount of health to heal (1 hp = 1/2 heart)
 */
void Cat::heal(int hp)
{
    // check if enough time has passed for a new animation state
    if (!newAnimationState(PET))
    {
        return;
    }

    int newHealth = getHealth() + hp;
    newHealth >= MAX_HP ? setHealth(MAX_HP) : setHealth(newHealth);
}

/**
 * Decrements health of cat.
 *
 * @param hp amount of health to deduct (1 hp = 1/2 heart)
 * @return true if health drops below 0, false if still alive
 */
bool Cat::takeDamage(int hp)
{
    int newHealth = getHealth() - hp;
    if (newHealth <= 0)
    {
        setHealth(0);
        return true;
    }
    setHealth(newHealth);
    return false;
}

/**
 * @brief handles collision for cat by checking for boxes, walls, and other objects
 *
 * @param map
 * @param i x coordinate of intended location
 * @param j y coordinate of intended location
 * @return true collision will occur
 * @return false no collision will occur
 */
bool Cat::handleCollision(std::shared_ptr<Map> map, int time, int i, int j)
{
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
    else if (map->findTile(objId)->getType() != Tile::Type::BOX)
    {
        // object that isn't a box
        return true; // collision
    }
    else
    {
        // object that is a box
        std::shared_ptr<Mobile> box = std::dynamic_pointer_cast<Mobile>(map->findTile(objId));
        if (box)
        {
            //  move returns true if object moves, which would remove the collision
            int newI = i;
            int newJ = j;
            switch (getDirection())
            {
            case UP:
                newJ--;
                break;
            case DOWN:
                newJ++;
                break;
            case LEFT:
                newI--;
                break;
            case RIGHT:
                newI++;
            }
            // std::cout << "(" << newI << ", " << newJ << ")\n";

            // potential object blocking box
            objId = map->getObjectTileIdWithXYCoordinates(newI, newJ);
            if (objId != 0 && map->findTile(objId)->getType() == RODENT)
            {
                // box pushing can also move rodents
                std::dynamic_pointer_cast<Rodent>(map->findTile(map->getObjectTileIdWithXYCoordinates(newI, newJ)))->move(map, getDirection(), time);
            }
            // move returns true if object moves, which would remove the collision
            if (box->move(map, getDirection(), time))
                return false;
            else
                return true;
        }
    }
    return false;
}

/**
 * @brief Acts as a wrapper for the Mobile::move method to
 * implement the cat move cooldown
 *
 * @param map allows move to check collision
 * @param dir move direction
 * @param time change in time from last game loop in milliseconds
 * @return true cat moved
 * @return false cat could not move
 */
bool Cat::catMove(std::shared_ptr<Map> map, Direction dir, int time)
{
    // TODO: implement cat move cooldown
    // if (getType() == Tile::Type::CAT)
    //    std::cout << "cat\n";
    // std::cout << moveCooldown << "in move\n";
    // if (moveCooldown >= 0)
    //    return false;
    // std::cout << getMoveCooldown() << " move cooldown\n";
    // cooldown has to have been reset to 0 to move the cat
    if (newAnimationState(MOVE))
    {
        return move(map, dir, time);
    }

    return false; // didn't move
}

/**
 * Updates Cat cooldowns
 *
 * @param map the shared map object
 * @param deltaMs amount of time passed since last game loop, in milliseconds
 */
void Cat::update(std::shared_ptr<Map> map, int deltaMs)
{
    // Convert milliseconds to seconds so we can subtract from the cooldown doubles
    double delta = deltaMs / 1000.0;
    //  adjust the move cooldown based on time elapsed if moving
    setAnimationDuration(getAnimationDuration() + delta * getSpeed());
    if (getAnimationDuration() >= 1) // move cooldown should be between 0 and 1. reset to 0 if over 1.
    {
        setAnimationDuration(0);
        animationState = IDLE; // movement finished
    }

    // Update cooldown timers
    double curScratch = getScratchCooldown() - delta;
    if (curScratch <= 0.0)
    {
        setScratchAmmo(3);
        setScratchCooldown(0.0);

    }
    else
    {
        if (curScratch > 0.0 && curScratch <= 3.0)
        {
            setScratchAmmo(2);
        }
        else if (curScratch > 3.0 && curScratch <= 6.0)
        {
            setScratchAmmo(1);
        }
        else if (curScratch > 6.0)
        {
            setScratchAmmo(0);
        }
        setScratchCooldown(curScratch);
    }

    double curHiss = getHissCooldown() - delta;
    curHiss <= 0 ? setHissCooldown(0) : setHissCooldown(curHiss);
}