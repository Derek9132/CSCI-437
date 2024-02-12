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

actor::actor(void)
{
    imageFilePath = "";

    boundingBox = setBoundingBox();

    vec2D placeholder(0.0f, 0.0f);

    directionVelocity = placeholder;

    position = placeholder;

}



SDL_Rect actor::setBoundingBox() //must be overwritten
{
    SDL_Rect placeholder;
    return placeholder;
}

bool actor::collide(const &actor) const
{
    if (SDL_HasIntersection(&boundingBox, &actor.boundingBox))
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



