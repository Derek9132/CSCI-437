#include "paddle.h"

class paddle : public actor

paddle::paddle(int dir)
{
    boundingBox =  setBoundingBox();

};
    direction = dir;

    directionVelocity = setDirectionVelocity();
}

SDL_Rect setBoundingBox()
{

}
