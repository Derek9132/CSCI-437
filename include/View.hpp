#ifndef VIEW_HPP
#define VIEW_HPP
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include "Logic.hpp"
#include "Map.hpp"
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include "AnimationManager.hpp"
#include "TitleScreen.hpp"
#include "CutsceneManager.hpp"

#define SCREEN_W 960
#define SCREEN_H 704

class View
{
private:
    // tileIds of info bar sprites on png
    static const int FULL_HEART_ID = 231;
    static const int HALF_HEART_ID = 232;
    static const int EMPTY_HEART_ID = 233;
    static const int ITEM_EMPTY = 234;
    static const int ITEM_VIAL = 235;
    static const int ITEM_THERMOMETER = 236;
    static const int ITEM_PLANT = 237;
    static const int ITEM_BEAKER = 238;

    static const int SCRATCH_EMPTY = 241;
    static const int SCRATCH_FULL = 250;
    static const int HISS_EMPTY = 251;
    static const int HISS_FULL = 256;

    SDL_Renderer *renderer;
    SDL_Texture *cat;
    SDL_Texture *tileTexture;
    SDL_Texture *screenTexture;
    SDL_Texture *screenRocketTexture;
    TTF_Font *font;
    SDL_Rect ogRect;
    SDL_Rect dstRect;
    SDL_Rect attackRect;
    SDL_Rect screenRectOg;        // stores dimensions of screen for start, pause, and end screens
    const SDL_Rect screenRectDst; // destination on screen doesn't change

    std::shared_ptr<TitleScreen> titleScreen;
    std::shared_ptr<InfoScreen> pauseScreen;
    std::shared_ptr<InfoScreen> dieScreen;
    std::shared_ptr<InfoScreen> ownerDieScreen;
    std::shared_ptr<InfoScreen> winScreen;
    std::shared_ptr<CutsceneManager> cutsceneManager;

    int w, h;           // width and height of tile png
    const int tileSize; // size of the tile in pixels
    // Calculates x and y position of tile on png using tileId
    int calcX(int tileId) { return ((tileId - 1) % (w / tileSize)) * tileSize; };
    int calcY(int tileId) { return ((tileId - 1) / (w / tileSize)) * tileSize; };

    int mapTopLeft[2]; // top left tile of the map displayed calculated based on cat location

    std::shared_ptr<AnimationManager> animationManager;
    double frame = 0;
    int isMoving = 0;
    bool isPaused = false;

    void csci437_error(const std::string &msg);
    std::string talk = "";
    void csci437_mix_error(const std::string &msg);
    void renderScreen(int pngX, int pngY);
    void renderButtons(std::shared_ptr<InfoScreen> screen, int mouseX, int mouseY);
    bool buttonKeyOverlap(std::shared_ptr<InfoScreen::Button>, int mouseX, int mouseY);

public:
    View(SDL_Renderer *renderer, int tileSize, int catX, int catY);

    void renderTile(const std::shared_ptr<Tile> tile, int tileId, int i, int j, int deltaMs);
    void renderInfoBar(std::shared_ptr<Cat> cat, bool vial, bool therm, bool plant, bool beak);
    void renderMap(const std::shared_ptr<Map> map, int deltaMs);
    void renderDialogBox(std::string dialog);
    void calcTopLeft(int catX, int catY, int mapWidth, int mapHeight);

    bool handleStartScreen(std::shared_ptr<Logic> logic, int deltaMs);
    bool handleEndScreen(std::shared_ptr<Logic> logic, int deltaMs);
    bool handlePauseScreen(std::shared_ptr<Logic> logic, int deltaMs);
    bool handleDeadScreen(std::shared_ptr<Logic> logic, int deltaMs);

    bool update(std::shared_ptr<Logic> logic, int deltaMs); // returns game state
};

#endif