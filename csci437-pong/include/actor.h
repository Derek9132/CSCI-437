#ifndef ACTOR_H
#define ACTOR _H


#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <string>
#include "vec2D.h"

/**
SUPERCLASS

Each actor has
    -position (2D vector)
    -speed and direction (2D vector)
    -bounding box (SDL_Rect)
    -strings to define file path for texture (needed when drawing window)

Each child of actor must implement
    -setBoundingBox (will be different for each actor)
    -getPosition()
    -setPosition()

Each actor can implement
    -collide(const &actor)

**/


class actor
{
public:

    //Constructors
    actor(void)

    //Members
    const std::string imageFilePath;

    const SDL_Rect boundingBox;

    vec2D position;

    vec2D directionVelocity;

    //Methods
    virtual bool collide(const &actor) const;

    virtual void setBoundingBox();

    SDL_Rect getBoundingBox();

    vec2D getPosition();

    vec2D getDirectionVelocity();

};

#endif ACTOR_H
