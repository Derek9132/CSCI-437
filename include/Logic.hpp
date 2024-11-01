#ifndef LOGIC_HPP
#define LOGIC_HPP
#include "Tile.hpp"
#include "Map.hpp"
#include "Mobile.hpp"
#include "Immobile.hpp"
#include "Cat.hpp"
#include "Item.hpp"
#include "Rodent.hpp"
#include "Scientist.hpp"
#include "stdio.h"
#include "../json/include/nlohmann/json.hpp"
#include <memory>
#include <string>
#include <vector>
#include "Item.hpp"

class Logic
{
private:
    std::shared_ptr<Map> map;
    std::shared_ptr<Map> originalMap; // Used for resetting puzzles
    std::shared_ptr<Map> saveMap = nullptr;
    std::shared_ptr<Cat> cat;
    std::shared_ptr<Rodent> ownerRodent = std::make_shared<Rodent>(136, 4, 16, 10, 64);
    //std::shared_ptr<Rodent> ownerRodent;
    std::shared_ptr<Scientist> ownerScientist;
    int gameState[4] = {0};
    std::shared_ptr<Rodent> rodentArray[20];
    int nextRodentIdx = 0;
    int saveGameState[4] = {0};
    const int tileSize = 64;
    bool finalPuzzleAdded = false;

    double autoResetPuzzleElapsedTime = 0.0; // Total time since last puzzle reset in seconds
    int autoResetPuzzleFrequnecy = 30; // The number of seconds it takes for the next puzzle to reset
    int autoNextPuzzleId = 1;

    double timeSpentCurPuzzle = 0.0; // Total time spent in same puzzle in seconds
    int stuckPuzzleResetFrequency = 100; // The number of second it takes for the puzzle reset dialogue to appear
    int currentPuzzleId = 0; // 0 if not in a puzzle
    bool stuck = false;
    bool isBossDead = false;
    bool isPlaying = false;
    bool isPaused = false;
    bool isCatDead = false;
    bool isTutorialCutscene = true;
    bool isPreBossCutscene = false;
    // Below for future cutscenes
    // bool isDeadPostBossCutscene = false;
    // bool isSavedPostBossCutscene = false;
    
    std::string dialogueArray[2][4][5] = {""};  // Holds strings for scientist dialogue
    int numLines[2][4] = {0};                   // Number of dialogue lines scientist has per state
    std::vector<std::string> tutorialDialogue;
    std::vector<std::string> preBossDialogue;
    int curDialogueIndex = 0;


    void createMap();
    void createOriginalMap();
    void readDialogue();
    std::shared_ptr<Tile> createBackgroundTile(int tileId, int x, int y);
    std::shared_ptr<Tile> createObjectTile(int tileId, int uniqueId, int x, int y);

    void resetGame();
    //void resetPuzzle(int puzzleId); uncomment later
    void autoResetPuzzleHandler(int deltaMs);
    void stuckResetPuzzleHandler(int deltaMs);
    void resetCat();
    void resetRodents();
    std::shared_ptr<Rodent> createBoss();

    bool isPuzzleVisible(int puzzleId);

public:

    Logic();

    void setGameStateIndex(int value, int index) { this->gameState[index] = value; };
    void setBossBool(bool bossBool) {this->isBossDead = bossBool; }; // TODO: Delete later, for testing
    void setIsFinal(int num);
    void setIsPlaying(bool isPlaying) {this->isPlaying = isPlaying; };
    void setIsPaused(bool isPaused) { this->isPaused = isPaused; };
    void setIsPlayerDead(bool isDead) { this->isCatDead = isDead; };
    void setIsCutscene(bool isCutscene);
    void setIsTutorialCutscene(bool isTutorialCutscene) {this->isTutorialCutscene = isTutorialCutscene; };
    void setIsPreBossCutscene(bool isPreBossCutscene) {this->isPreBossCutscene = isPreBossCutscene; };
    void setCurDialogueIndex(int index) {this->curDialogueIndex = index; };
    void hideFinalPuzzle();
    void showFinalPuzzle();
    bool isFinal();

    std::shared_ptr<Map> getMap() const { return map; };
    std::shared_ptr<Cat> getCat() const { return cat; };
    std::shared_ptr<Rodent> getOwnerRodent() const { return ownerRodent; };
    int getGameStateIndex(int index) const {return gameState[index]; };
    int* getGameState() { return gameState; };
    int getTileSize() const { return tileSize; };
    bool getBossDead() const { return isBossDead; };
    bool getIsPaused() const { return isPaused; };
    bool getIsCatDead() const { return isCatDead; };
    int getCurDialogueIndex() const { return curDialogueIndex; };
    std::vector<std::string> getTutorialDialogue () const { return tutorialDialogue; };
    std::vector<std::string> getPreBossDialogue() const { return preBossDialogue; };
    bool isPlayerPlaying() const { return isPlaying; };  
    bool isGameOver();
    bool isCutscene() const;

    void handleCatDeath();
    void saveGame(); // move to private later
    void loadSave(); // move to private later
    void reloadGame();
    void playAgain();
    
    void resetPuzzle(int puzzleId); 
    int getCurrentPuzzleIdPosition(); // 0 means player is not within the boundaries of a puzzle  
    
    void itemInteract();
    std::string scientistInteract();
    std::string tutorialInteract();
    std::string preBossInteract();
    bool itemAdjacent();
    bool scientistAdjacent();

    bool update(int deltaMs);
};

#endif