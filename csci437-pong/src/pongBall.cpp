#include "pongBall.h"

pongBall::pongBall(void)
{
    imageFilePath = "../resource/ball.png";

    boundingBox = setBoundingBox();

    directionVelocity = vec2D dV(0,5);

    position = vec2D pos(30,30);
}


/**reflect method
takes vec2D as argument
uses the collided object's directionVelocity vec2D to calculate ball's new direction
ball's new directionVelocity vec2D will be set to the collided object's vec2D
new vec2D will be multiplied by magnitude of ball's previous directionVelocity vector (speed)
random value will be added to ball's vec2D
**/
