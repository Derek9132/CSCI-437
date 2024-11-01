#include "Scientist.hpp"

/**
 * @brief Construct a new Scientist:: Scientist object
 * 
 * @param tileId 
 * @param x 
 * @param y 
 * @param size 
 * @param id 
 * @param gameState 
 */
Scientist::Scientist(int tileId, int x, int y, int size, int id, int *gameState) 
: Immobile(tileId, x, y, size, id)
{   
    if (tileId == 61) 
        scientistNumber = 1;    // Seb
    if (tileId == 81)
        scientistNumber = 2;    // Janelle
    if (tileId == 71)
        scientistNumber = 3;    // Colleen
    if (tileId == 51)
        scientistNumber = 4;    // Derek

    dialogueIndex = 0;
    doneTalking = false;
};

/**
 * @brief stores string input at the specified index in the before array
 * @param index
 * @param input
*/
void Scientist::storeStringBefore(int index, std::string input)
{
    before[index] = input;
}

/**
 * @brief stores string input at the specified index in the after array
 * @param index
 * @param input
*/
void Scientist::storeStringAfter(int index, std::string input)
{
    after[index] = input;
}

void Scientist::speak(int numLines)
{
    dialogueIndex++;

    if (dialogueIndex > numLines)
    {
        doneTalking = true;
        dialogueIndex = 0;
    }
    else
    {
        doneTalking = false;
    }
}

bool Scientist::forceScientistMove(std::shared_ptr<Map> map, Direction dir, int time) {

    // i is future x location, j is future y location
    int i = getX();
    int j = getY();
    int tileId = map->getObjectTileIdWithXYCoordinates(i, j);

    switch (dir)
    {
        case UP:
            j -= 1; // reminder that up is actually decreasing y values
            setDirection(DOWN);
            break;
        case DOWN:
            j += 1;
            setDirection(DOWN);
            break;
        case LEFT:
            i -= 1;
            setDirection(DOWN);
            break;
        case RIGHT:
            i += 1;
            setDirection(DOWN);
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