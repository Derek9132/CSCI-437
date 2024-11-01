// Using SDL and standard IO
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "../include/View.hpp"
#include "../include/Logic.hpp"
#include <emscripten.h>

// Screen dimension constants
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 704;

void csci437_error(const std::string &msg)
{
    std::cerr << msg << " (" << SDL_GetError() << ")" << std::endl;
    exit(0);
}

void csci437_ttf_error(const std::string &msg)
{
    std::cerr << msg << " (" << TTF_GetError() << ")" << std::endl;
    exit(0);
}

/**
 * @brief Get the Elapsed Time object
 * Check what the current time is compared to previous time
 *
 * @param init when initializing, just return current time
 * @param prevTime results from previously calling getElapsedTime
 * @return int milliseconds changed
 */
int getElapsedTime(bool init, int prevTime)
{
    if (init)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - prevTime;
}

// Global variables to keep state
bool running = true;
bool is_player_dead = false; // Activates intermediate "start over or load save?" screen
SDL_Renderer *renderer;
std::shared_ptr<Logic> logic;
std::shared_ptr<View> view;
uint64_t prevTime;

void main_loop() 
{
    // if it's been less than ten milliseconds, wait
    while (getElapsedTime(false, prevTime) < 1) 
    {
        SDL_Delay(10);
    }

    // Check for start or end screen conditions
    if (!logic->isPlayerPlaying())
    {
        if (logic->getIsPaused())
        {
            running = view->handlePauseScreen(logic, getElapsedTime(false, prevTime));
        }
        else if (logic->getIsCatDead())
        {
            running = view->handleDeadScreen(logic, getElapsedTime(false, prevTime));
        }
        else if (logic->isFinal()) // End game mode/screen
        {
            running = view->handleEndScreen(logic, getElapsedTime(false, prevTime));
        }
        else // Start game mode/screen
        {
            running = view->handleStartScreen(logic, getElapsedTime(false, prevTime));
        }
    }
    else
    {
        // playing loop
        is_player_dead = logic->update(getElapsedTime(false, prevTime));
        running = view->update(logic, getElapsedTime(false, prevTime));
        prevTime = getElapsedTime(true, 0); // set previous time for next round
    }

    // Check if the main loop should continue running
    if (!running) {
        emscripten_force_exit(0); // Stop the main loop if running is false
    }
}

int main(int argc, char **argv)
{
    /*** Initialization ***/

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        csci437_error("SDL could not initialize!");

    if (TTF_Init() < 0)
        csci437_ttf_error("Unable to initialize TTF!");

    // Initialize SDL for audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        csci437_error("SDL audio could not initialize!");

    // Initialize SDL Mixer for .wav files
    if (Mix_Init(0) < 0)
        csci437_error("SDL mixer could not initialize!");

    // Create window
    SDL_Window *window = SDL_CreateWindow("CSCI-437 Skeleton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
        csci437_error("Window could not be created!");

    // Init Bitmap loading
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
        csci437_error("SDL could not initialize bitmap loaders!");

    // Small delay to allow the system to create the window.
    SDL_Delay(100);

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (renderer == NULL)
        csci437_error("Unable to create renderer!");

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    logic = std::make_shared<Logic>();
    view = std::make_shared<View>(renderer, logic->getTileSize(), logic->getCat()->getX(), logic->getCat()->getY());

    // Initialize prevTime
    prevTime = getElapsedTime(true, 0);

    /*** Main Loop ***/
    emscripten_set_main_loop(main_loop, 0, 1); // Emscripten main loop

    /*** Clean Up ***/
    // close any open audio files
    Mix_CloseAudio();

    // quick SDL_mixer
    Mix_Quit();

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    // Done.
    return 0;
}

