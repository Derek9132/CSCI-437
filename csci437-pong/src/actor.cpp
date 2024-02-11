#include "actor.h"

/**
SUPERCLASS

Each actor has
    -position (2D vector)
    -speed and direction (2D vector)
    -bounding box (SDL_Rect)
    -strings to define file path for texture (needed when drawing window)

Each child of actor must implement
    -setBoundingBox (will be different for each actor)

Each actor can implement
    -collide(const &actor)

**/

actor::actor(void);
{
    imageFilePath = "";

    boundingBox = nullptr;

    directionVelocity = vec2D dV(0,0);

    position = vec2D dV(0,0);

}


SDL_Rect actor::getBoundingBox()
{
    return boundingBox;
}

virtual void actor::setBoundingBox() //must be overwritten
{
    boundingBox = nullptr;
}

virtual bool actor::collide(const &actor) const
{
    if (SDL_IntersectRect(boundingBox, actor.boundingBox))
    {
        return true;
    }
    return false;
}

vec2D actor::getPosition()
{
    return position;
}

vec2D actor::getDirectionVelocity()
{
    return directionVelocity;
}



