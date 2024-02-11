#include "logic.h"

/**
Handles:
    -player input
    -window (background, walls, paddles, ball)
    -initialization
    -shutdown
    -score
    -human view update method
Requests
    -ball position (from logic)
    -paddle position (from logic)
    -update method (from logic)

**/

class humanView
{
public:

    humanView( logic * gameLogic );

    void update(int elapsedTime);


};

