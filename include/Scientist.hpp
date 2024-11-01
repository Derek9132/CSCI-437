#ifndef SCIENTIST_HPP
#define SCIENTIST_HPP
#include "Immobile.hpp"
#include <string>

class Scientist : public Immobile
{
private:
    /**
     * Scientist
     * NUMBER   NAME        PLACE       ITEM        TILE ID     Direction
     * 
     * 1        Seb         Rightmost   Vial        61          0 Up
     * 2(Lead)  Janelle     Leftmost    Thermom     81          1 Down
     * 3        Colleen     Topmost     Plant       71          2 Left
     * 4        Derek       Middle      Beaker      51          3 Right
    */

    int dialogueIndex;
    bool doneTalking;
    int scientistNumber;

    std::string before[5];
    std::string after[5];

public:
    Scientist(int tileId, int x, int y, int size, int id, int *gameState);

    void speak(int numLines);

    int getDialogueIndex() { return dialogueIndex; };
    int getScientistNumber() { return scientistNumber; }; 

    void storeStringBefore(int index, std::string input);
    void storeStringAfter(int index, std::string input);

    bool isDoneTalking() { return doneTalking; };

    bool forceScientistMove(std::shared_ptr<Map> map, Direction dir, int time);
};

#endif