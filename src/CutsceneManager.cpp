#include "CutsceneManager.hpp"

CutsceneManager::CutsceneManager() {
    talk = "";
    isPaused = false;
    isQuit = false;
    readCutsceneActionLists();
}

void CutsceneManager::readCutsceneActionLists() {
    std::ifstream ifs("../resource/CutsceneActionLists.json");
    nlohmann::json cutsceneActionLists = nlohmann::json::parse(ifs);

    auto tutorialActions = cutsceneActionLists["tutorial"];
    
    std::vector<int> tutorialActionList;

    for (int move : tutorialActions)
    {
        // Push new action list if not already in hashmap
        if (cutsceneIdtoCutsceneActionList.count(cutsceneId) != 1) {
            cutsceneIdtoCutsceneActionList[cutsceneId] = tutorialActionList;
        }
        // Store in existing action list
        cutsceneIdtoCutsceneActionList[cutsceneId].push_back(move);
    }

    cutsceneId++;

    auto preBossActions = cutsceneActionLists["preBoss"];
    
    std::vector<int> preBossActionList;

    for (int move : preBossActions)
    {
        // Push new action list if not already in hashmap
        if (cutsceneIdtoCutsceneActionList.count(cutsceneId) != 1) {
            cutsceneIdtoCutsceneActionList[cutsceneId] = preBossActionList;
        }
        // Store in existing action list
        cutsceneIdtoCutsceneActionList[cutsceneId].push_back(move);
    }

    cutsceneId = 0; // Reset back to 0
}

std::string CutsceneManager::handleCutscene(std::shared_ptr<Logic> logic, int deltaMs, SDL_Event e)
{
    // Keep track of time
    if (deltaMs > 100)
        return talk;

    cutsceneElapsedTimeSinceAction = (deltaMs / 1000.0) + cutsceneElapsedTimeSinceAction;

    // TODO: Read action loads arrays from a json on init, store in this hashmap, and use before handleCutscene switch statement
    // TODO: Get rid of word cutscene everywhere

    // Delay in movements (acts as a pause between big sets of movements)
    if (cutsceneElapsedTimeSinceAction >= cutsceneActionCooldown && cutsceneIdtoCutsceneActionList.count(cutsceneId) > 0)
    {
        // "Mod" time
        cutsceneElapsedTimeSinceAction -= cutsceneActionCooldown;
        std::vector<int> actionList = cutsceneIdtoCutsceneActionList[cutsceneId];

        return handleNextCutsceneAction(logic, deltaMs, e, actionList);
    }
    return talk;
}

std::string CutsceneManager::handleNextCutsceneAction(std::shared_ptr<Logic> logic, int deltaMs, SDL_Event e, std::vector<int> cutsceneActions)
{
    // NOTE: 1029 is the rodent fighting in cutscene. This is a unique ID that changes based on the json
    // NOTE: 1018 is the scientist in cutscene. This is a unique ID that changes based on the json

    // Gather objects
    // Execute next set of Actions, 1 action per object at a time per set
    // Breifly paueses at 0, acts as a cooldown between actions
    for (; cutsceneIndex < cutsceneActions.size() && cutsceneActions[cutsceneIndex] != 0; cutsceneIndex++)
    {

        switch (cutsceneActions[cutsceneIndex])
        {
        // Move cat up, down, left, or right
        case 1:
            logic->getCat()->catMove(logic->getMap(), Tile::Direction::UP, deltaMs);
            break;
        case 2:
            logic->getCat()->catMove(logic->getMap(), Tile::Direction::DOWN, deltaMs);
            break;
        case 3:
            logic->getCat()->catMove(logic->getMap(), Tile::Direction::LEFT, deltaMs);
            break;
        case 4:
            logic->getCat()->catMove(logic->getMap(), Tile::Direction::RIGHT, deltaMs);
            break;
        // Move owner up, down, left, or right
        case 5:
            logic->getOwnerRodent()->forceRodentMove(logic->getMap(), Tile::Direction::UP, deltaMs);
            break;
        case 6:
            logic->getOwnerRodent()->forceRodentMove(logic->getMap(), Tile::Direction::DOWN, deltaMs);
            break;
        case 7:
            logic->getOwnerRodent()->forceRodentMove(logic->getMap(), Tile::Direction::LEFT, deltaMs);
            break;
        case 8:
            logic->getOwnerRodent()->forceRodentMove(logic->getMap(), Tile::Direction::RIGHT, deltaMs);
            break;
        // Move lead scientist up, down, left, or right
        case 9:
        {
            Tile *tile = logic->getMap()->findTile(81).get();
            Scientist *scientist = dynamic_cast<Scientist *>(tile);
            scientist->forceScientistMove(logic->getMap(), Tile::Direction::UP, deltaMs);
            break;
        }
        case 10:
        {
            Tile *tile = logic->getMap()->findTile(81).get();
            Scientist *scientist = dynamic_cast<Scientist *>(tile);
            scientist->forceScientistMove(logic->getMap(), Tile::Direction::DOWN, deltaMs);
            break;
        }
        case 11:
        {
            Tile *tile = logic->getMap()->findTile(81).get();
            Scientist *scientist = dynamic_cast<Scientist *>(tile);
            scientist->forceScientistMove(logic->getMap(), Tile::Direction::LEFT, deltaMs);
            break;
        }
        case 12:
        {
            Tile *tile = logic->getMap()->findTile(81).get();
            Scientist *scientist = dynamic_cast<Scientist *>(tile);
            scientist->forceScientistMove(logic->getMap(), Tile::Direction::RIGHT, deltaMs);
            break;
        }
        // Move tutorial rodent up, down, left, or right
        case 13:
        {
            Tile *tile = logic->getMap()->findTile(1029).get();
            Rodent *rodent = dynamic_cast<Rodent *>(tile);
            rodent->forceRodentMove(logic->getMap(), Tile::Direction::UP, deltaMs);
            break;
        }
        case 14:
        {
            Tile *tile = logic->getMap()->findTile(1029).get();
            Rodent *rodent = dynamic_cast<Rodent *>(tile);
            rodent->forceRodentMove(logic->getMap(), Tile::Direction::DOWN, deltaMs);
            break;
        }
        case 15:
        {
            Tile *tile = logic->getMap()->findTile(1029).get();
            Rodent *rodent = dynamic_cast<Rodent *>(tile);
            rodent->forceRodentMove(logic->getMap(), Tile::Direction::LEFT, deltaMs);
            break;
        }
        case 16:
        {
            Tile *tile = logic->getMap()->findTile(1029).get();
            Rodent *rodent = dynamic_cast<Rodent *>(tile);
            rodent->forceRodentMove(logic->getMap(), Tile::Direction::RIGHT, deltaMs);
            break;
        }

        case 17:
        {
            // cat scratch
            logic->getCat()->forceScratch();
            break;
        }
        case 18:
        {
            // cat heal
            logic->getCat()->forcePet();
            break;
        }
        case 19:
        {
            // rodent bite
            Tile *tile = logic->getMap()->findTile(1029).get();
            Rodent *rodent = dynamic_cast<Rodent *>(tile);
            rodent->setDirection(Tile::Direction::LEFT);
            rodent->forceBite();
            break;
        }
        case 25:
        {
            // One piece of dialogue = two 25s in array. 
            // First 25 brings up the dialogue (else), second 25 waits for input (if).
            // One 25 in the array is dialogue that doesn't wait for user to press I before continuing
            if (isPaused)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_q)
                    {
                        setIsQuit(true);
                        return "";
                    }
                    // User skips cutscene
                    if (e.key.keysym.sym == SDLK_p)
                    {
                        isPaused = false;
                        logic->setIsCutscene(false);
                        return "";
                    }
                    // User interacts
                    if (e.key.keysym.sym == SDLK_i)
                    {
                        isPaused = false;
                    }
                }
            }
            else
            {
                isPaused = true;
                talk = logic->tutorialInteract();
            }
            break;
        }
        case 26:
        {
            // clear dialogue
            talk = "";
            isPaused = false;
            logic->setCurDialogueIndex(0);
            cutsceneId++;
            break;
        }
        // Move lead scientist to correct spot after tutorial cutscene
        case 27:
        {
            Tile *tile = logic->getMap()->findTile(81).get();
            Scientist *scientist = dynamic_cast<Scientist *>(tile);
            logic->getMap()->setObjectTileIdWithXYCoordinates(scientist->getX(), scientist->getY(), 0); // clear object from 3d array
            scientist->setX(8);
            scientist->setY(10);
            logic->getMap()->setObjectTileIdWithXYCoordinates(8, 10, 81); // update 3d array with new x, y
            break;
        }
        // Move cat, lead scientist, and owner to preBoss spots within the final puzzle
        case 28:
        {
            // Set cat spot
            logic->getCat()->setX(20);
            logic->getCat()->setY(5);
            logic->getMap()->setObjectTileIdWithXYCoordinates(logic->getCat()->getX(), logic->getCat()->getY(), logic->getCat()->getHashId());

            // Set lead scientists spot
            Tile *tile = logic->getMap()->findTile(81).get();
            Scientist *scientist = dynamic_cast<Scientist *>(tile);
            logic->getMap()->setObjectTileIdWithXYCoordinates(scientist->getX(), scientist->getY(), 0); // clear object from 3d array
            scientist->setX(20);
            scientist->setY(4);
            logic->getMap()->setObjectTileIdWithXYCoordinates(20, 4, 81); // update 3d array with new x, y

            // Set Other Scientists
            tile = logic->getMap()->findTile(71).get();
            scientist = dynamic_cast<Scientist *>(tile);
            logic->getMap()->setObjectTileIdWithXYCoordinates(scientist->getX(), scientist->getY(), 0); // clear object from 3d array
            scientist->setX(19);
            scientist->setY(4);
            logic->getMap()->setObjectTileIdWithXYCoordinates(19, 4, 71); // update 3d array with new x, y

            tile = logic->getMap()->findTile(61).get();
            scientist = dynamic_cast<Scientist *>(tile);
            logic->getMap()->setObjectTileIdWithXYCoordinates(scientist->getX(), scientist->getY(), 0); // clear object from 3d array
            scientist->setX(18);
            scientist->setY(4);
            logic->getMap()->setObjectTileIdWithXYCoordinates(18, 4, 61); // update 3d array with new x, y

            tile = logic->getMap()->findTile(51).get();
            scientist = dynamic_cast<Scientist *>(tile);
            logic->getMap()->setObjectTileIdWithXYCoordinates(scientist->getX(), scientist->getY(), 0); // clear object from 3d array
            scientist->setX(17);
            scientist->setY(4);
            logic->getMap()->setObjectTileIdWithXYCoordinates(17, 4, 51); // update 3d array with new x, y

            break;
        }
        case 29:
        {
            // One piece of dialogue = two 25s in array. 
            // First 25 brings up the dialogue (else), second 25 waits for input (if).
            // One 25 in the array is dialogue that doesn't wait for user to press I before continuing
            if (isPaused)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_q)
                    {
                        setIsQuit(true);
                        return "";
                    }
                    // User skips cutscene
                    if (e.key.keysym.sym == SDLK_p)
                    {
                        isPaused = false;
                        logic->setIsCutscene(false);
                        return "";
                    }
                    // User interacts
                    if (e.key.keysym.sym == SDLK_i)
                    {
                        isPaused = false;
                    }
                }
            }
            else
            {
                isPaused = true;
                talk = logic->preBossInteract();
            }
            break;
        }
        case 30:
        {
            // clear dialogue
            talk = "";
            isPaused = false;
            logic->setCurDialogueIndex(0);
            cutsceneId++;
            break;
        }
        }
        // User requests quit or skip
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_q)
            {
                setIsQuit(true);
                return "";
            }
            // Skip cutscene
            if (e.key.keysym.sym == SDLK_p)
            {
                isPaused = false;
                logic->setIsCutscene(false);
                return "";
            }
        }
    }
    if (isPaused)
    {
        cutsceneIndex--; // stay paused until input is received
    }
    if (cutsceneActions[cutsceneIndex] == 0)
    {
        cutsceneIndex++;
    }
    if (cutsceneIndex >= cutsceneActions.size())
    {
        logic->setIsCutscene(false);
        cutsceneIndex = 0;
    }
    return talk;
}