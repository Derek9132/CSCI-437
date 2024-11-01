#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include <map>
#include "Tile.hpp"
#include "Cat.hpp"
#include <string.h>
#include <iostream>


class AnimationManager
{

private:

    struct Animation {

        int startingFrame;
        double speed;
        int numberOfFrames;

        Animation(int n): startingFrame(n),
            speed(1.0), numberOfFrames(3) {}

    }; 
    
    typedef struct Animation animation;

    /** ratio is a number between 0.0 and 1.0 that represents the progression through animation frames. 
     * the ratio is calculated by the elapsed time in deltaMs and changed in the update method.
     * for example: a frameRatio of 0.0 to 0.33 for an animation with 3 frames would result in the first frame,
     * from 0.33 to 0.66, the second frame, and from 0.66 to 0.99 the third frame. */
    double frameRatio = 0.0;
    double speed = 1.0;

    std::shared_ptr<Animation> animationArray[5][7][4] = {};

public:
    AnimationManager();

    void update(int deltaMs);
    int playAnimation(const std::shared_ptr<Tile> tile, double frameRatio);
    int findNextFrameIncrement(double ratio, int numberOfFrames);
};

#endif
