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

logic::logic(void)
{
    //building new object with all actors
    logic::actorList = {pongBall ball, paddle playerPaddle(0), paddle aiPaddle(1), scoreZone playerZone(0), scoreZone aiZone(1), wall topWall(2), wall bottomWall(3)};

}


logic::update()
void logic::update(int elapsedTime)
{
    for (int i = 0; i < 7; i++)
    {

        //newPos = currentPos + elapsedTime * direction
    }
    //iterate through actor list
    //call getPosition() on each actor, notify view
    //gets position of each actor
    //check for collision, reflect if ball collided

    //calculate new position of ball using elapsedTime
    //calculate new position of paddles using elapsedTime


    /**if (actorList[0].collide(paddle) || actorlist[0].collide(wall))
    {

