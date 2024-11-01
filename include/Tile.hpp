#ifndef TILE_HPP
#define TILE_HPP

#include <SDL.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <memory>
#include <vector>

class Tile
{
public:
    enum Type
    {
        BACKGROUND,
        WALL,
        BOX,
        CAT,
        RODENT,
        OWNER,
        SCIENTIST,
        ITEM
    }; 
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    enum AnimationState
    {
        IDLE,
        MOVE,
        ATTACK,
        HISS,
        POUNCE,
        DIE,
        PET
    };

private:
    /** this is the location of the tile on the PNG in format rowcolumn, 
     * which is used as the map hashmap ID for background tiles*/
    
    int x, y;
    int size;
    Type type; 

protected:
    int tileId;
    Direction direction;
    AnimationState animationState;
    bool isOwner = false;

public:
    Tile(int tileId, int x, int y, int size, Direction direction = Direction::DOWN);

    int getTileId() const { return tileId; };
    int getX() const { return x; };
    int getY() const { return y; };
    virtual enum Direction getDirection() const { return direction; };
    enum Type getType() const { return type; };
    enum AnimationState getState() const { return animationState; };
    virtual const int getHashId() const { return tileId; };
    bool IsBoss() { return isOwner; };
    void setOwner(bool b) { isOwner = b; };

    void setTileId(int tileId) { this->tileId = tileId; };
    void setX(int x) { this->x = x; };
    void setY(int y) { this->y = y; };
    void setDirection(enum Direction direction) { this->direction = direction; };
    void setAnimationState(enum AnimationState state) { this->animationState = state; };
    
    enum Type tileIdToType(int tileId);
};

#endif