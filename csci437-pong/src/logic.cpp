#include "logic.h"
#include <iostream>
#include <SDL.h>

/**
Handles
    -actor list (ball, playerPaddle, aiPaddle, scoreZone, wall)
    -updating actor positions and notifying view (score zone and walls will not move)
    -collision detection
    -logic update method

Requests
    -actor positions (using getPos() method from actor interface)
    -actor speeds (using getSpeed() method from actor interface)
    -collide() method from ball/actor

Other
    -every time the update method runs, logic must get positions of each actor and change positions of ball and paddles
    -view will tell logic to move actors
    -How to use speed variable to change position
**/

logic::logic(void) { logic::actorList = {pongBall ball, paddle playerPaddle, paddle aiPaddle, scoreZone zone}; } //building new object with all actors


logic::update()
{
    //iterate through actor list
    //call getPosition() on each actor, notify view

    /**if (actorList[0].collide(paddle) || actorlist[0].collide(wall))
    {
        actorlist[0].reflect() //reflect() method from ball
    }**/


}
