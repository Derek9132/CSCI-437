#include "aiView.h"
#include "humanView.h"
#include <SDL_image.h>
#include <SDL.h>

humanView::humanView( logic * gameLogic )
{
     gameLogic = nullptr; //constructor
}


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
Other
    -humanView only needs to know location of ball and blue paddle
    -repaint entire window every frame
    -How to make humanView responsible for only one paddle when both views take same logic pointer as argument

**/


void csci437_error(const std::string& msg)
{
  std::cerr << msg << " (" << SDL_GetError() << ")" << std::endl;
  exit(0);
}

void csci437_img_error(const std::string& msg)
{
  std::cerr << msg << " (" << IMG_GetError() << ")" << std::endl;
  exit(0);
}

void humanView::update(int elapsedTime) //humanView update method uses player input
{
    //adaptive frame rate

    //input handling to move paddle

    //
}

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

int main(int argc, char** argv)
{
    //Load in logic and players
    logic playLogic; //both views take same logic as parameter; both views have identical info on ball's position

    humanView player1(&playLogic);

    aiView player2(&playLogic);

    /**     Initialize window     **/

    //Build window
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) csci437_error("SDL could not initialize!");

    SDL_Window* window = SDL_CreateWindow( "CSCI-437 Skeleton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( window == NULL ) csci437_error("Window could not be created!");

    if( IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG ) < 0 ) csci437_img_error("SDL could not initialize bitmap loaders!");

    SDL_Delay(100);

    //Load background
    SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL) csci437_error("Unable to create renderer!");

    SDL_Surface* background = IMG_Load("../resource/board.png");
    if(background == NULL) csci437_img_error("Could not image!");

    SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, background );
    if(texture == NULL) csci437_error("Could not create texture from surface!");

    SDL_FreeSurface( background );
    background = NULL;

    //Render background
    SDL_Rect dest;

    dest.x = 0;
    dest.y = 0
    dest.w = SCREEN_WIDTH;
    dest.w = SCREEN_HEIGHT;

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    //Load paddles

    //blue paddle controlled by human


    //red paddle controlled by computer


    //Load walls/score zones


    //Load ball
    SDL_Surface* ball = IMG_Load(playLogic.actorList[0].imageFilePath);
    if(ball == NULL) csci437_img_error("Could not image!");

    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface( renderer, ball );
    if(ballTexture == NULL) csci437_error("Could not create ball texture from surface!");

    SDL_FreeSurface( ball );
    ball = NULL;

    SDL_RenderCopy(renderer, ballTexture, NULL, &playLogic.actorList[0].boundingBox);

    //Load scoreboard


    bool running = true;

    while (running) //Main game loop
    {
        //repaint window
        //player1.update();
        //player2.update();
        //Use SDL_UpdateWindowSurface();
    }
    /*** Clean Up ***/

    SDL_DestroyTexture( texture );

    SDL_DestroyRenderer( renderer );

    SDL_DestroyWindow( window );

    IMG_Quit();
    SDL_Quit();

    // Done.
    return 0;

}
