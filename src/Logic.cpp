#include <iostream>
#include <fstream>
#include "Logic.hpp"

Logic::Logic()
{
    readDialogue();
    createMap();
    createOriginalMap();
    hideFinalPuzzle();
}

/**
 * @brief Read map from json, create background and object tiles,
 * and store in internal map object. Also creates and stores puzzle
 * boundary coordinates in seperate internal map.
 */
void Logic::createMap()
{
    map = std::make_shared<Map>();

    // Read internal map from json
    std::ifstream ifs("../resource/map.json");
    nlohmann::json mapData = nlohmann::json::parse(ifs);

    // Map dimensions: 32x44
    auto layers = mapData["layers"];
    int numCols = layers[0]["width"].get<int>();
    int numRows = layers[0]["height"].get<int>();
    map->setMapX(numCols);
    map->setMapY(numRows);

    // Create and add background tile layer to map
    auto backgroundLayer = layers[0];
    auto backgroundLayerData = backgroundLayer["data"];

    int backgroundX = 0;
    int backgroundY = 0;
    for (int id : backgroundLayerData)
    {
        if (backgroundX >= numCols)
        {
            // Update coordinates to first column of the next row
            backgroundX = backgroundX % numCols;
            backgroundY++;
        }
        std::shared_ptr<Tile> newTile = createBackgroundTile(id, backgroundX, backgroundY);
        map->storeTile(id, newTile);                                             // store tile in hashmap
        map->setBackgroundTileIdWithXYCoordinates(backgroundX, backgroundY, id); // store tile id in 3d array

        backgroundX++;
    }

    // Create and add object tile layer to map
    auto objectLayer = layers[1];
    auto objectLayerData = objectLayer["data"];

    int objectX = 0;
    int objectY = 0;
    int uniqueIdCtr = 1000;
    for (int id : objectLayerData)
    {
        if (objectX >= numCols)
        {
            // Update coordinates to first column of the next row
            objectX = objectX % numCols;
            objectY++;
        }
        if (id != 0) // 0 represents no object occupying that tile
        {
            std::shared_ptr<Tile> newTile = createObjectTile(id, uniqueIdCtr, objectX, objectY);
            // Scientists are not movable so don't get assigned unique id
            if (newTile->getType() != Tile::Type::SCIENTIST) {
                map->storeTile(uniqueIdCtr, newTile);                                 // store tile in hashmap
                map->setObjectTileIdWithXYCoordinates(objectX, objectY, uniqueIdCtr); // store tile id in 3d array
            }
            else {
                map->storeTile(id, newTile);
                map->setObjectTileIdWithXYCoordinates(objectX, objectY, id); // store tile id in 3d array
            }
            uniqueIdCtr++;
        }
        else
        {
            // 0 represents no object occupying that tile
            map->setObjectTileIdWithXYCoordinates(objectX, objectY, 0);
        }
        objectX++;
    }

    // Create and add puzzle boundaries to map
    auto puzzleBoundaryLayer = layers[2];
    auto puzzleBoundaryLayerData = puzzleBoundaryLayer["data"];

    int puzzleBoundaryLayerX = 0;
    int puzzleBoundaryLayerY = 0;
    for (int id : puzzleBoundaryLayerData)
    {
        if (puzzleBoundaryLayerX >= numCols)
        {
            // Update coordinates to first column of the next row
            puzzleBoundaryLayerX = puzzleBoundaryLayerX % numCols;
            puzzleBoundaryLayerY++;
        }
        // Store puzzle corner coordinates in hashmap of puzzle boundary coordinates
        if (id != 0)
        {
            map->storePuzzleBoundaryCoordinates(puzzleBoundaryLayerX, puzzleBoundaryLayerY, id);
        }
        puzzleBoundaryLayerX++;
    }

    return;
}

/**
 * @brief Create a copy of the 3d tile array that stores each tile's unique Id
 * and store in in seperate internal map
 *
 */
void Logic::createOriginalMap()
{
    // Store only object locations
    originalMap = std::make_shared<Map>();
    for (int x = 0; x < map->getMapX(); x++)
    {
        for (int y = 0; y < map->getMapY(); y++)
        {
            int id = map->getObjectTileIdWithXYCoordinates(x, y);
            originalMap->setObjectTileIdWithXYCoordinates(x, y, id);
        }
    }
}

/**
 * @brief hides the final puzzle at the beginning of the game. Runs once
 */
void Logic::hideFinalPuzzle()
{

    // puzzle id of last puzzle
    std::vector<std::pair<int, int>> coordinatesVector = map->getPuzzleBoundaryCoordinates(5);

    // Get puzzle boundaries
    std::pair<int, int> upperLeft = coordinatesVector[0];
    std::pair<int, int> bottomRight = coordinatesVector[3];

    for (int x = upperLeft.first; x < bottomRight.first + 1; x++)
    {
        for (int y = upperLeft.second; y < bottomRight.second + 1; y++)
        {
            int tileId = originalMap->getObjectTileIdWithXYCoordinates(x, y);
            map->setObjectTileIdWithXYCoordinates(x, y, tileId);

            if (tileId != 0)
            {

                map->setObjectTileIdWithXYCoordinates(x, y, 0);
            }
        }
    }

    finalPuzzleAdded = false;
}

/**
 * @brief shows the final puzzle once all 4 items have been collected
 * Moves owner into puzzle with correct animations
 */
void Logic::showFinalPuzzle()
{

    // puzzle id of last puzzle
    std::vector<std::pair<int, int>> coordinatesVector = map->getPuzzleBoundaryCoordinates(5);

    // Get puzzle boundaries
    std::pair<int, int> upperLeft = coordinatesVector[0];
    std::pair<int, int> bottomRight = coordinatesVector[3];

    for (int x = upperLeft.first; x < bottomRight.first + 1; x++)
    {
        for (int y = upperLeft.second; y < bottomRight.second + 1; y++)
        {
            int tileId = originalMap->getObjectTileIdWithXYCoordinates(x, y);
            map->setObjectTileIdWithXYCoordinates(x, y, tileId);
        }
    }

    map->setObjectTileIdWithXYCoordinates(ownerScientist->getX(), ownerScientist->getY(), 0); // delete ownerScientist

    std::shared_ptr<Rodent> boss = createBoss();

    ownerRodent = boss;

    map->storeTile(6000, boss);                                 // store tile in hashmap
    map->setObjectTileIdWithXYCoordinates(16, 10, 6000);


    rodentArray[nextRodentIdx] = boss;

    /**int ownerId = originalMap->getObjectTileIdWithXYCoordinates(ownerRodent->getX(), ownerRodent->getY());

    ownerRodent->setX(16);
    ownerRodent->setY(10);

    ownerRodent->setAnimationState(Tile::AnimationState::MOVE);
    ownerRodent->setDirection(Tile::Direction::DOWN);

    rodentArray[nextRodentIdx] = ownerRodent;

    // std::cout<<rodentArray[nextRodentIdx]->getTileId();

    map->setObjectTileIdWithXYCoordinates(16, 10, 136);**/

    finalPuzzleAdded = true;
}

/**
 * @brief Reads in dialogue data from dialogue json
 */
void Logic::readDialogue()
{
    std::ifstream ifs("../resource/Dialogue2.json");
    nlohmann::json dialogueData = nlohmann::json::parse(ifs);

    auto gameStates = dialogueData["gamestates"];

    for (int i = 0; i < gameStates.size(); i++)
    {
        auto scientists = gameStates[i];

        for (int j = 0; j < scientists.size(); j++)
        {
            int k = 0;

            auto scientistLines = scientists["scientist" + std::to_string(j + 1)];

            for (std::string s : scientistLines)
            {
                dialogueArray[i][j][k] = s;
                numLines[i][j]++;
                k++;
            }
        }
    }

    auto tutorial = dialogueData["tutorial"];
    for (int i = 0; i < tutorial.size(); i++)
    {
        tutorialDialogue.push_back(tutorial[i]);
    }

    auto preBoss = dialogueData["preBoss"];
    for (int i = 0; i < preBoss.size(); i++) 
    {
        preBossDialogue.push_back(preBoss[i]);
        //std::cout<<preBoss[i];
    }
}

/**
 * @brief Create a tile object for the background or wall to make the map
 *
 * @param tileId determines whether the tile is a background or a wall
 * @param x
 * @param y
 * @return std::shared_ptr<Tile>
 */
std::shared_ptr<Tile> Logic::createBackgroundTile(int tileId, int x, int y)
{
    Tile::Type type;
    return std::make_shared<Tile>(tileId, x, y, 64);
}

std::shared_ptr<Rodent> Logic::createBoss()
{
    std::shared_ptr<Rodent> boss = std::make_shared<Rodent>(136, 6000, 16, 10, 64);
    return boss;
}

/**
 * @brief Handle game state when cat interacts with item
 * After itemAdjacent is true
 * Called from logic based on user input
 */
void Logic::itemInteract()
{
    // error checking in case there isn't an item
    if (!itemAdjacent())
        return;

    int x, y; // coordinates of item
    x = cat->getX();
    y = cat->getY();
    switch (cat->getDirection())
    {
    case Tile::Direction::UP:
        y -= 1;
        break;
    case Tile::Direction::DOWN:
        y += 1;
        break;
    case Tile::Direction::LEFT:
        x -= 1;
        break;
    case Tile::Direction::RIGHT:
        x += 1;
    }

    Tile *tile = map->findTile(map->getObjectTileIdWithXYCoordinates(x, y)).get();
    Item *item = dynamic_cast<Item *>(tile);
    int id;
    if (item != nullptr)
    {
        item->setIsCollected(true);
        id = item->getId();
    }
    map->setObjectTileIdWithXYCoordinates(x, y, 0);
    setGameStateIndex(1, id - 1);

    tile = NULL;
    item = NULL;
}

/**
 * @brief Handles interactions between the player/cat by checking if they are interacting with a scientist.
 * getting the scientist's next peice of dialogue based on the gameState, and returning itthe scientist's next string to speak
 *
 * @return String object containing the scientist's next peice of dialogue
 */
std::string Logic::scientistInteract()
{
    std::string talk = "";
    // error checking in case there isn't an item
    if (!scientistAdjacent())
        return "";

    int x, y; // coordinates of scientist
    x = cat->getX();
    y = cat->getY();
    switch (cat->getDirection())
    {
    case Tile::Direction::UP:
        y -= 1;
        break;
    case Tile::Direction::DOWN:
        y += 1;
        break;
    case Tile::Direction::LEFT:
        x -= 1;
        break;
    case Tile::Direction::RIGHT:
        x += 1;
    }

    Tile *tile = map->findTile(map->getObjectTileIdWithXYCoordinates(x, y)).get();
    Scientist *scientist = dynamic_cast<Scientist *>(tile);
    int num = scientist->getScientistNumber() - 1;
    if (scientist != nullptr)
    {
        talk = dialogueArray[gameState[num]][num][scientist->getDialogueIndex()];
        scientist->speak(numLines[gameState[num]][num]); // update dialogueIndex and doneTalking
    }
    cat->heal(18); // heal cat fully when talking to scientist
    saveGame();    // save game when talking to scientist

    return talk;
}

void Logic::setIsCutscene(bool isCutscene) {
    isTutorialCutscene = false;
    isPreBossCutscene = false;
}

std::string Logic::tutorialInteract()
{
    std::string talk = "";

    if (curDialogueIndex < tutorialDialogue.size())
    {
        talk = tutorialDialogue[curDialogueIndex];
        curDialogueIndex++;
    }
    return talk;
}

std::string Logic::preBossInteract() // crashes the game
{
    std::string talk = "";

    if (curDialogueIndex < preBossDialogue.size())
    {
        talk = preBossDialogue[curDialogueIndex];
        curDialogueIndex++;
    }
    return talk;
}

/**
 * @brief Check if cat is adjacent to and facing an item
 * Used to display text encouraging the player to interact
 * Called from logic
 * @return true cat is adjacent to and facing an item
 * @return false cat is not adjacent to or not facing an item
 */
bool Logic::itemAdjacent()
{
    int x, y; // coordinates of item
    x = cat->getX();
    y = cat->getY();
    switch (cat->getDirection())
    {
    case Tile::Direction::UP:
        y -= 1;
        break;
    case Tile::Direction::DOWN:
        y += 1;
        break;
    case Tile::Direction::LEFT:
        x -= 1;
        break;
    case Tile::Direction::RIGHT:
        x += 1;
    }

    int uniqueId = map->getObjectTileIdWithXYCoordinates(x, y);
    if (uniqueId != 0)
    {
        std::shared_ptr<Tile> tile = map->findTile(uniqueId);
        if (tile->getType() == Tile::Type::ITEM)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Check if cat is adjacent to and facing a scientist
 * Used to display text encouraging the player to interact
 * Called from logic
 * @return true cat is adjacent to and facing a scientist
 * @return false cat is not adjacent to or not facing a scientist
 */
bool Logic::scientistAdjacent()
{
    int x, y; // coordinates of item
    x = cat->getX();
    y = cat->getY();
    switch (cat->getDirection())
    {
    case Tile::Direction::UP:
        y -= 1;
        break;
    case Tile::Direction::DOWN:
        y += 1;
        break;
    case Tile::Direction::LEFT:
        x -= 1;
        break;
    case Tile::Direction::RIGHT:
        x += 1;
    }

    int uniqueId = map->getObjectTileIdWithXYCoordinates(x, y);
    if (uniqueId != 0)
    {
        std::shared_ptr<Tile> tile = map->findTile(uniqueId);
        if (tile->getType() == Tile::Type::SCIENTIST)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Determine the type of object based on the tile ID and create a tile object
 * If object is a scientist, assigns dialogue to the scientist
 * @param tileId location of tile on png
 * @param uniqueId unique ID for map hashmap
 * @param x location of tile on map
 * @param y location of tile on map
 * @return std::shared_ptr<Tile> new tile object
 */
std::shared_ptr<Tile> Logic::createObjectTile(int tileId, int uniqueId, int x, int y)
{
    std::shared_ptr<Tile> tile = nullptr;
    switch (tileId) // Functions as an id and corresponds to location in respective png
    {
    case 31:
        tile = std::make_shared<Mobile>(tileId, uniqueId, x, y, 64, true);
        break;
    case 41:
        tile = std::make_shared<Item>(tileId, x, y, 64, 1);
        break;
    case 42:
        tile = std::make_shared<Item>(tileId, x, y, 64, 2);
        break;
    case 43:
        tile = std::make_shared<Item>(tileId, x, y, 64, 3);
        break;
    case 44:
        tile = std::make_shared<Item>(tileId, x, y, 64, 4);
        break;
    case 51:
    {   // Scientist: Derek
        // change id from 0 to scientistCount
        tile = std::make_shared<Scientist>(tileId, x, y, 64, 0, gameState);
        Tile *tile_ptr = tile.get();
        Scientist *scientist4 = dynamic_cast<Scientist *>(tile_ptr);
        for (int i = 0; i < 5; i++)
        {
            scientist4->storeStringBefore(i, dialogueArray[0][0][i]);
        }
        for (int i = 0; i < 5; i++)
        {
            scientist4->storeStringAfter(i, dialogueArray[1][0][i]);
        }
        break;
    }
    case 61:
    {   // Scientist: Seb
        // change id from 0 to scientistCount
        tile = std::make_shared<Scientist>(tileId, x, y, 64, 0, gameState);
        Tile *tile_ptr_1 = tile.get();
        Scientist *scientist1 = dynamic_cast<Scientist *>(tile_ptr_1);
        for (int i = 0; i < 5; i++)
        {
            scientist1->storeStringBefore(i, dialogueArray[0][1][i]);
        }
        for (int i = 0; i < 5; i++)
        {
            scientist1->storeStringAfter(i, dialogueArray[1][1][i]);
        }
        break;
    }
    case 71:
    {   // Scientist: Colleen
        // change id from 0 to scientistCount
        tile = std::make_shared<Scientist>(tileId, x, y, 64, 0, gameState);
        Tile *tile_ptr_2 = tile.get();
        Scientist *scientist3 = dynamic_cast<Scientist *>(tile_ptr_2);
        for (int i = 0; i < 5; i++)
        {
            scientist3->storeStringBefore(i, dialogueArray[0][2][i]);
        }
        for (int i = 0; i < 5; i++)
        {
            scientist3->storeStringAfter(i, dialogueArray[1][2][i]);
        }
        break;
    }
    case 81:
    {   // Scientist: Janelle
        // change id from 0 to scientistCount
        tile = std::make_shared<Scientist>(tileId, x, y, 64, 0, gameState);
        Tile *tile_ptr_3 = tile.get();
        Scientist *scientist2 = dynamic_cast<Scientist *>(tile_ptr_3);
        for (int i = 0; i < 5; i++)
        {
            scientist2->storeStringBefore(i, dialogueArray[0][3][i]);
        }
        for (int i = 0; i < 5; i++)
        {
            scientist2->storeStringAfter(i, dialogueArray[1][3][i]);
        }
        break;
    }
    case 91:
    {
        std::shared_ptr<Rodent> rodent = std::make_shared<Rodent>(tileId, uniqueId, x, y, 64);
        rodentArray[nextRodentIdx] = rodent;
        nextRodentIdx++;
        tile = rodent;
        break;
    }
    case 131:
    {
        ownerScientist = std::make_shared<Scientist>(tileId, x, y, 64, 0, gameState);
        ownerRodent = std::make_shared<Rodent>(tileId, uniqueId, x, y, 64);
        tile = ownerScientist;
        break;
    }
    case 136:
    {
        ownerRodent = std::make_shared<Rodent>(tileId, uniqueId, x, y, 64);
        tile = ownerRodent;
        break;
    }
    case 171:
    {
        cat = std::make_shared<Cat>(tileId, uniqueId, x, y, 64);
        tile = cat;
        break;
    }
    }
    return tile;
}

/**
 * @brief Determines if the game is over by checking if all of the items were collected
 * and if the boss is dead. If either of these are not met, the game is not over
 *
 * @return true game is over
 * @return false game is not over
 */
bool Logic::isGameOver()
{
    if (!isFinal())
        return false;

    // Boss is dead
    if (isBossDead)
    {
        return true;
    }
    return false;
}

bool Logic::isFinal()
{
    // All items collected
    for (int i = 0; i < 4; i++)
    {
        if (gameState[i] == 0)
        {
            return false;
        }
    }
    // setIsPreBossCutscene(true); // worst case scenario: comment this out, just show owner cured screen
    isPlaying = false;
    return true;
}

void Logic::setIsFinal(int num) {
    for (int i = 0; i < 4; i++)
    {
        gameState[i] = num;
    }
}

/**
 * @brief Called only when the player chooses to play again after the end screen.
 * The game is reset completely and so are the conditions isPlaying and isBossDead to false so
 * the end conditions for the game are reset properly.
 *
 */
void Logic::playAgain()
{ // TODO: Transition into start title screen instead of back into game
    resetGame();
    isPlaying = true;
    isBossDead = false;
    for (int i = 0; i < 4; i++)
    {
        if (getGameStateIndex(i) != 0)
        {
            setGameStateIndex(0, i);
        }
    }
}

/**
 * @brief Used by handleCatDeath and reload saved games from pause and cat death screens.
 *
 */
void Logic::reloadGame()
{
    if (saveMap)
    {
        // Soft Reset: Load Save
        loadSave();
        resetCat();
        resetRodents();
    }
    else
    {

        // Hard Reset: Start Game Over
        resetGame();
    }
}

/**
 * @brief Triggered when the cat's health reaches 0 or below 0
 * Implements a Hard (Reset entire game) or Soft Reset (Load last saved checkpoint)
 * by checking if a saved map exists
 *
 */
void Logic::handleCatDeath()
{
    reloadGame();
    setIsPlayerDead(false);
    setIsPlaying(true);
}

/**
 * @brief Saves the current 3d array from the internal map into a seperate internal saved map object,
 * but only saved the tildIds in the 3d array. Also copies over the items collected by copying the gameState
 *
 */
void Logic::saveGame()
{
    // Triggered when the player interacts with a scientist or item
    saveMap = std::make_shared<Map>();
    saveMap->setMapX(map->getMapX());
    saveMap->setMapY(map->getMapY());
    for (int x = 0; x < saveMap->getMapX(); x++)
    {
        for (int y = 0; y < saveMap->getMapY(); y++)
        {
            int id = map->getObjectTileIdWithXYCoordinates(x, y);
            saveMap->setObjectTileIdWithXYCoordinates(x, y, id);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        saveGameState[i] = gameState[i];
    }
}

/**
 * @brief Loads the most revent save by iterating through the internal saved map's
 * 3d array, copying the tileId's over to the same location in the current map, and
 * updating the internal object's x and y coordinates in the current map's hashmap.
 * Also loads in the old gameState into the new one
 *
 */
void Logic::loadSave()
{
    // Soft reset
    // Triggered when the cat dies and a saved map exists

    // Reset internal map with the save by iterating through the saved map and updating
    // the actual map contents with the saved version
    for (int x = 0; x < saveMap->getMapX(); x++)
    {
        for (int y = 0; y < saveMap->getMapY(); y++)
        {
            // Find tileid in save
            int id = saveMap->getObjectTileIdWithXYCoordinates(x, y);

            // if (id == 1031)
            //     std::cout << saveMap->findTile(1031)->getX() << ", " << map->findTile(1031)->getY() << "\n";
            //  Replace the location the tileId is stored in the actual map's 3d array
            //  with the location in the save map's 3d array
            map->setObjectTileIdWithXYCoordinates(x, y, id);

            if (id != 0)
            { // id = 0 means no object occupies that tile
                // Update internal object locations to match 3d array
                std::shared_ptr<Tile> object = map->findTile(id);
                object->setX(x);
                object->setY(y);

                // Reset health of cats and rodents
            }
        }
    }

    // Load saved game state to actual game state
    for (int i = 0; i < 4; i++)
    {
        gameState[i] = saveGameState[i];
    }
}

/**
 * @brief Performs a hard reset, loading in the deafult map, objects, and reseting the gameState
 *
 */
void Logic::resetGame()
{
    // Hard reset
    // Triggered when:
    // - the player completes the game or
    // - the cat dies and no saved map exists

    nextRodentIdx = 0;
    for (int i = 0; i < 14; i++)
    {
        rodentArray[i] = nullptr;
    }

    createMap();
    gameState[0] = 0;
    gameState[1] = 0;
    gameState[2] = 0;
    gameState[3] = 0;

    nextRodentIdx = 0;
}

/**
 * @brief Performs a hard reset, loading in the deafult map, objects, and reseting the gameState.
 * Resets all objects except for dead rodents.
 * Triggered for puzzles that do not have their items collected when;
 * - the player hasn't moved after x amount of time and chooses to reset the current puzzle or
 * - the puzzle is out of line of sight for x amount of time
 *
 */
void Logic::resetPuzzle(int puzzleId)
{

    // Reset the puzzle
    // Key = Puzzle #
    std::vector<std::pair<int, int>> coordinatesVector = map->getPuzzleBoundaryCoordinates(puzzleId);

    // Get puzzle boundaries
    std::pair<int, int> upperLeft = coordinatesVector[0];
    std::pair<int, int> bottomRight = coordinatesVector[3];

    // Reset all objects except for dead rodents
    for (int x = upperLeft.first; x < bottomRight.first; x++)
    {
        for (int y = upperLeft.second; y < bottomRight.second; y++)
        {
            int tileId = originalMap->getObjectTileIdWithXYCoordinates(x, y);
            map->setObjectTileIdWithXYCoordinates(x, y, tileId);

            if (tileId != 0)
            {
                Tile *tile = map->findTile(tileId).get();

                // Reset only rodents that are alive
                if (tile->getType() == Tile::Type::RODENT)
                {
                    Rodent *rodent = dynamic_cast<Rodent *>(tile);
                    if (rodent->getHealth() > 0.0)
                    {
                        rodent->setX(x);
                        rodent->setY(y);
                    }
                }
                // Reset all other objects
                else
                {
                    tile->setX(x);
                    tile->setY(y);
                }
            }
        }
    }
}

/**
 * @brief Resets all of the cat's internal attributes such as health and cooldowns
 *
 */
void Logic::resetCat()
{
    cat->resetCat();
}

/**
 * @brief Iterates through rodent array and resets each Rodent's internal attributes such as isAggro, health, and cooldown
 *
 */
void Logic::resetRodents()
{
    // Map reset handles position in map position and internal object x and y coordinates
    // Take advantage of future data structure containing all rodents

    for (int i = 0; i < 14; i++)
    {
        rodentArray[i]->resetRodent();
    }
}

/**
 * @brief Resets one puzzle by checking if enough time has passed
 * and if the player/cat cannot view the puzzle too. On the next
 * call, it will attempt to reset a different puzzle.
 *
 */
void Logic::autoResetPuzzleHandler(int deltaMs)
{

    // Keep track of time
    autoResetPuzzleElapsedTime = (deltaMs / 1000.0) + autoResetPuzzleElapsedTime;

    // Periodically reset one puzzle
    if (autoResetPuzzleElapsedTime > autoResetPuzzleFrequnecy)
    {
        // "Mod" time
        autoResetPuzzleElapsedTime -= autoResetPuzzleFrequnecy;

        // Check if puzzle is visible to player
        if (!isPuzzleVisible(autoNextPuzzleId))
        {
            resetPuzzle(autoNextPuzzleId);
        }

        autoNextPuzzleId %= 4;
        autoNextPuzzleId++;
    }
}

/**
 * @brief Changes internal bool stuck after checking if enough time has passed
 * since the player has been in the puzzle and not moved outside of it
 *
 */
void Logic::stuckResetPuzzleHandler(int deltaMs)
{

    int newCurrentPuzzleId = getCurrentPuzzleIdPosition();
    double elapsedTime = (deltaMs / 1000.0);
    // Player is in a puzzle
    if (newCurrentPuzzleId != 0)
    {
        timeSpentCurPuzzle += elapsedTime;
        if (timeSpentCurPuzzle > stuckPuzzleResetFrequency)
        {
            timeSpentCurPuzzle -= stuckPuzzleResetFrequency;
            stuck = true;
        }
    }
    else
    {
        timeSpentCurPuzzle = 0.0;
        stuck = false;
    }

    currentPuzzleId = newCurrentPuzzleId;
}

/**
 * @brief Checks if the puzzle located by the puzzleId is within the
 * visible limits of the cat/player
 *
 * @return true if puzzle is visible
 * @return false if puzzle not visible
 */
bool Logic::isPuzzleVisible(int puzzleId)
{

    int viewableTopLeftCoordinates[2] = {};
    int viewableBottomRightCoordinates[2] = {};
    int maxScreenTilesX = 15;
    int maxScreenTilesY = 11;
    int mapHeight = 32;
    int mapWidth = 44;
    int catX = cat->getX();
    int catY = cat->getY();

    // Get puzzle boundaries
    std::vector<std::pair<int, int>> puzzleCoordinatesVector = map->getPuzzleBoundaryCoordinates(puzzleId);
    std::pair<int, int> puzzleTopLeftCoordiantes = puzzleCoordinatesVector[0];
    std::pair<int, int> puzzleBottomRightCoordiantes = puzzleCoordinatesVector[3];

    // Get viewable tile coordinates
    // set upper left x coordinate
    if (catX - (maxScreenTilesX / 2) <= 0) // handle edge case on left
        viewableTopLeftCoordinates[0] = 0;
    else if (catX + (maxScreenTilesX / 2) >= mapWidth) // handle edge case on right
        viewableTopLeftCoordinates[0] = mapWidth - maxScreenTilesX;
    else
        viewableTopLeftCoordinates[0] = catX - (maxScreenTilesX / 2);

    // set upper left y coordinate
    if (catY - (maxScreenTilesY / 2) <= 0) // handle edge case on top
        viewableTopLeftCoordinates[1] = 0;
    else if (catY + (maxScreenTilesY / 2) >= mapHeight) // handle edge case on bottom
        viewableTopLeftCoordinates[1] = mapHeight - maxScreenTilesY;
    else
        viewableTopLeftCoordinates[1] = catY - (maxScreenTilesY / 2);

    // Use existing size and the new coordinates to calcualte the bottom right tile coordinates
    viewableBottomRightCoordinates[0] = viewableTopLeftCoordinates[0] + maxScreenTilesX;
    viewableBottomRightCoordinates[1] = viewableTopLeftCoordinates[1] + maxScreenTilesY;

    // Check for visible collision between viewable tiles and tiles in puzzle (similar to checking for collision)
    // Only a collision if the range of BOTH x's and y's of player viewing screen and puzzle boundaries overlap
    // For no overlapping x's
    if (viewableBottomRightCoordinates[0] < puzzleTopLeftCoordiantes.first)
    {
        return false;
    }
    if (viewableTopLeftCoordinates[0] > puzzleBottomRightCoordiantes.first)
    {
        return false;
    }
    // For no overlapping y's
    if (viewableBottomRightCoordinates[1] < puzzleTopLeftCoordiantes.second)
    {
        return false;
    }
    if (viewableTopLeftCoordinates[1] > puzzleBottomRightCoordiantes.second)
    {
        return false;
    }

    // Past this point, there has to be a collision
    return true;
}

bool Logic::isCutscene() const
{
    if (isTutorialCutscene || isPreBossCutscene)
    { //|| isDeadPostBossCutscene || isSavedPostBossCutscene)
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Uses the cats current location to determine which puzzle the cat is currently located in
 *
 * @return 0 if the cat/player is not in a puzzle
 * @return puzzleId representing the puzzle the cat is located in, corresponding to internal puzzle hashmap in internal map
 */
int Logic::getCurrentPuzzleIdPosition()
{
    int catX = cat->getX();
    int catY = cat->getY();

    // Only in puzzle if the range of BOTH x's and y's of the players positions and puzzle boundaries overlap
    for (int puzzleId = 1; puzzleId <= map->getNumberOfPuzzles(); puzzleId++)
    {
        // Get puzzle boundaries
        std::vector<std::pair<int, int>> puzzleCoordinatesVector = map->getPuzzleBoundaryCoordinates(puzzleId);
        std::pair<int, int> puzzleTopLeftCoordiantes = puzzleCoordinatesVector[0];
        std::pair<int, int> puzzleBottomRightCoordiantes = puzzleCoordinatesVector[3];

        // Only in puzzle if the position of the player is within the puzzle boundaries
        if (catX >= puzzleTopLeftCoordiantes.first && catX <= puzzleBottomRightCoordiantes.first &&
            catY >= puzzleTopLeftCoordiantes.second && catY <= puzzleBottomRightCoordiantes.second)
        {
            return puzzleId;
        }
    }
    return 0;
}

/**
 * @brief Update the Logic side of the game
 * Uses internal map object to access and update tile objects
 *
 * @param deltaMs change in time since last game loop in milliseconds
 * @return true the player/cat has died
 * @return false the player/cat is still alive
 */
bool Logic::update(int deltaMs)
{
    //autoResetPuzzleHandler(deltaMs);
    stuckResetPuzzleHandler(deltaMs);

    cat->update(map, deltaMs);
    if (!isCutscene())
    {
        // update rodents
        if (finalPuzzleAdded == false)
        {
            for (int i = 0; i < 14; i++)
            {
                if (rodentArray[i]->getHealth() > 0)
                {
                    // don't update dead rodents
                    rodentArray[i]->update(map, cat, deltaMs);
                    //if (finalPuzzleAdded)
                        //std::cout<<rodentArray[14]->getTileId();
                }
            }
        }
        else
        {
            rodentArray[14]->update(map, cat, deltaMs);
        }
    }

    // Check if player died
    if (cat->getHealth() <= 0.0)
    {
        // handleCatDeath();
        setIsPlayerDead(true);
        setIsPlaying(false);
        return true;
    }

    // TODO: et up conditions show end screens render from view within possible end scenes
    // Check if all items have been collected and sets up conditions for pre boss cutscene
    if (isFinal())
    {
        // TODO: implement owner specific update method to update owner animation state (like cat->update or rodent->update)
        
        if (!finalPuzzleAdded)
        {
            showFinalPuzzle();
            finalPuzzleAdded = true;
        }
        if (getOwnerRodent()->getHealth() <= 0)
        {
            isBossDead = true;
            // set owner dead cutscene
        }
        else if (getOwnerRodent()->isConfined(map))
        {
            //std::cout<<"confined";
        }
        // TODO: if (getOwnerRodent()->isConfined(map)
        // set cure cutscene
    }
    // else 
    // {
    //     // If between 1-3 items collected, change owner animation to sick
    //     for (int i = 0; i < 4; i++) 
    //     {s
    //         if (gameState[i] == 1)
    //             ownerRodent->setAnimationState(Tile::AnimationState::DIE);
    //             break;
    //     }
    // }

    return false;
}