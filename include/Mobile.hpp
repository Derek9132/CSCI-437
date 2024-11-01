#ifndef MOBILE_HPP
#define MOBILE_HPP
#include "Tile.hpp"
#include "Map.hpp"

class Mobile : public Tile
{
private:
    double speed;
    double moveCooldown;
    double moveCooldownElapsedTime;
    bool isBox;
    int uniqueId; // this is location of the mobile object in the hashmap because each needs its own state
    double animationDuration;

protected:
    void setSpeed(double speed) { this->speed = speed; };
    void setMoveCooldown(double moveCooldown) { this->moveCooldown = moveCooldown; };
    void setMoveCooldownElapsedTime(double moveCooldownElapsedTime) { this->moveCooldownElapsedTime = moveCooldownElapsedTime; };
    void setAnimationDuration(double animationDuration) { this->animationDuration = animationDuration; };
    // void setAnimationState(AnimationState newState) { this->animationState = newState; };

    double getSpeed() const { return speed; };
    double getMoveCooldown() { return moveCooldown; };
    double getMoveCooldownElapsedTime() { return moveCooldownElapsedTime; };

    virtual bool handleCollision(std::shared_ptr<Map> map, int time, int i, int j);
    bool newAnimationState(AnimationState newState); 

public:
    //virtual bool handleCollision(std::shared_ptr<Map> map, int time, int i, int j);
    Mobile(int tileId, int uniqueId, int x, int y, int size, bool isBox);

    double getAnimationDuration() { return animationDuration; };
    AnimationState getAnimationState() const { return animationState; };
    virtual enum Direction getDirection() const override { return direction; };
    virtual const int getHashId() const override { return uniqueId; };

    virtual bool move(std::shared_ptr<Map> map, Direction dir, int time);
    virtual void update(std::shared_ptr<Map> map, int time) {};
};

#endif