#ifndef ITEM_HPP
#define ITEM_HPP
#include "Immobile.hpp"

class Item : public Immobile
{
private:
    bool isCollected;

public:
    Item(int tileId, int x, int y, int size, int id)
    : Immobile(tileId, x, y, size, id) { this->isCollected = false; };

    bool getIsCollected() { return isCollected; };
    void setIsCollected(bool isCollected) { this->isCollected = isCollected; };

    void collect() { isCollected = true; };
};

#endif