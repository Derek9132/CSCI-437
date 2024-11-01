#include "View.hpp"

void View::csci437_error(const std::string &msg)
{
    std::cerr << msg << " (" << SDL_GetError() << ")" << std::endl;
    exit(0);
}

void View::csci437_mix_error(const std::string &msg)
{
    std::cerr << msg << " (" << Mix_GetError() << ")" << std::endl;
    exit(0);
}

View::View(SDL_Renderer *renderer, int tileSize, int catX, int catY) : renderer(renderer), tileSize(tileSize), screenRectDst((SDL_Rect){0, 0, 960, 704})
{
    // initialize audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    // set up audio file as music
    Mix_Music *music = Mix_LoadMUS("../resource/game_full.wav");
    if (!music)
        csci437_mix_error("Could not load audio.");

    Mix_PlayMusic(music, -1); // loop indefinitely
    music = NULL;             // memory management

    // initialize font
    font = TTF_OpenFont("../resource/Arial.ttf", 25);
    if (font == NULL)
        csci437_error("Unable to open font!");

    // initialize and save screen textures
    SDL_Surface *screens = IMG_Load("../resource/screens.png");
    if (screens == NULL)
        csci437_error("Could not image screens.");
    screenTexture = SDL_CreateTextureFromSurface(renderer, screens);
    if (screenTexture == NULL)
        csci437_error("Could not create screens texture.");
    SDL_FreeSurface(screens);
    screens = NULL;

    // initialize and save rocket texture for title screen
    SDL_Surface *rocket = IMG_Load("../resource/rocket.png");
    if (rocket == NULL)
        csci437_error("Could not image rocket.");
    screenRocketTexture = SDL_CreateTextureFromSurface(renderer, rocket);
    if (screenRocketTexture == NULL)
        csci437_error("Could not create rocket texture; ");
    SDL_FreeSurface(rocket);
    rocket = NULL;

    // initialize and save game textures
    SDL_Surface *tileSet = IMG_Load("../resource/tileset.png");
    if (tileSet == NULL)
        csci437_error("Could not load image!");

    tileTexture = SDL_CreateTextureFromSurface(renderer, tileSet);
    if (tileTexture == NULL)
        csci437_error("Could not create texture from surface!");

    SDL_FreeSurface(tileSet);
    tileSet = NULL;

    // Get width and height of png from tile texture
    SDL_QueryTexture(tileTexture, NULL, NULL, &w, &h);

    ogRect = (SDL_Rect){0, 0, tileSize, tileSize};
    dstRect = (SDL_Rect){0, 0, tileSize, tileSize};
    attackRect = (SDL_Rect){0, 0, 96, 96};
    animationManager = std::make_shared<AnimationManager>();

    // create screen objects for pause, ownerDieEnd, start, win, die
    titleScreen = std::make_shared<TitleScreen>(0, 1408, 1920, 1408);            // play button coords
    pauseScreen = std::make_shared<InfoScreen>(960, 0, 1920, 1636, true);        // resume button coords
    dieScreen = std::make_shared<InfoScreen>(1920, 0, 1920, 1862, false);        // reload button coords
    ownerDieScreen = std::make_shared<InfoScreen>(1920, 704, 1920, 1749, false); // replay button coords
    winScreen = std::make_shared<InfoScreen>(960, 704, 1920, 1749, false);       // replay button coords
    
    // create cutscene manager
    cutsceneManager = std::make_shared<CutsceneManager>();

    // create SDL_Rect for screen
    screenRectOg = (SDL_Rect){0, 0, 960, 704};
}

/**
 * Renders one tile on the map.
 *
 * @param tile the tile object to render
 * @param i the x-position of the tile on the map
 * @param j the y-position of the tile on the map
 * @param deltaMs
 */
void View::renderTile(const std::shared_ptr<Tile> tile, int tileId, int i, int j, int deltaMs)
{
    int rectWidth, rectHeight;

    if (tile->getType() >= 3) // If not background tile
    {
        // handle getting unique animation ratio from cat for movement
        double tempRatio = 0.0;
        if (tile->getType() == 3 || tile->getType() == 4 || tile->IsBoss())
        {
            tempRatio = std::dynamic_pointer_cast<Mobile>(tile)->getAnimationDuration();
        }
        tileId = animationManager->playAnimation(tile, tempRatio);
    }

    ogRect.x = ((tileId - 1) % (w / tileSize)) * tileSize;
    ogRect.y = ((tileId - 1) / (w / tileSize)) * tileSize;

    dstRect.x = i * tileSize;
    dstRect.y = j * tileSize;

    SDL_RenderCopy(renderer, tileTexture, &ogRect, &dstRect);
}

/**
 * Renders the visible area of the map.
 *
 * @param map
 * @param deltaMs
 */
void View::renderMap(const std::shared_ptr<Map> map, int deltaMs)
{
    int tempId;
    std::shared_ptr<Tile> tempTile;
    for (int i = mapTopLeft[0]; i < mapTopLeft[0] + 15; i++)
    {
        for (int j = mapTopLeft[1]; j < mapTopLeft[1] + 11; j++)
        {
            tempId = map->getBackgroundTileIdWithXYCoordinates(i, j);
            tempTile = map->findTile(tempId);
            renderTile(tempTile, tempTile->getTileId(), i - mapTopLeft[0], j - mapTopLeft[1], deltaMs);
            tempId = map->getObjectTileIdWithXYCoordinates(i, j);
            if (tempId > 0)
            {
                tempTile = map->findTile(tempId);
                renderTile(tempTile, tempTile->getTileId(), i - mapTopLeft[0], j - mapTopLeft[1], deltaMs); // cat gets rendered here
            }
        }
    }
}

/**
 * Renders the info bar. Hearts (health) live in top left, attack cooldowns
 * in top right, and items collected so far in bottom middle.
 *
 * @param cat shared pointer to Cat to get health
 * @param beak true if the beaker has been collected, false if not
 * @param therm true if the thermometer has been collected, false if not
 * @param plant true if the plant has been collected, false if not
 * @param vial true if the vial of blood has been collected, false if not
 */
void View::renderInfoBar(std::shared_ptr<Cat> cat, bool vial, bool therm, bool plant, bool beak)
{
    int i, j, gap, size, xOffset, dstFrmEdge;

    /*******************
     ** RENDER HEARTS **
     ** TOP LEFT      **
     *******************/

    // Max HP is 18, represented by 9 hearts, so calculate number
    // of full hearts to display and whether we need a half heart
    int fullHearts = floor(cat->getHealth() / 2);
    bool halfHeart = cat->getHealth() % 2 == 0 ? false : true;

    ogRect.x = calcX(FULL_HEART_ID);
    ogRect.y = calcY(FULL_HEART_ID);
    dstFrmEdge = 8, gap = 56;
    dstRect.y = dstFrmEdge;
    for (i = 0; i < fullHearts; i++)
    {
        xOffset = i * gap + dstFrmEdge;
        dstRect.x = xOffset;
        SDL_RenderCopy(renderer, tileTexture, &ogRect, &dstRect);
    }
    if (halfHeart)
    {
        ogRect.x = calcX(HALF_HEART_ID);
        ogRect.y = calcY(HALF_HEART_ID);
        dstRect.x = i * gap;
        SDL_RenderCopy(renderer, tileTexture, &ogRect, &dstRect);
    }

    /********************
     ** RENDER ATTACKS **
     ** TOP RIGHT      **
     *******************/

    // Convert multi-second cooldowns to a ratio between 0.0 and 1.0 for animation
    double scratchRatio = cat->getScratchCooldown() / (cat->SCRATCH_COOLDOWN_TIME * 3);
    double hissRatio = cat->getHissCooldown() / cat->HISS_COOLDOWN_TIME;

    // Find which cooldown frame to render
    int scratchFrame = SCRATCH_FULL - animationManager->findNextFrameIncrement(scratchRatio, 10);
    int hissFrame = HISS_FULL - animationManager->findNextFrameIncrement(hissRatio, 6);

    size = 96, dstFrmEdge = 4;
    // Scratch
    ogRect.x = calcX(scratchFrame);
    ogRect.y = calcY(scratchFrame);
    attackRect.x = SCREEN_W - size - dstFrmEdge;
    attackRect.y = dstFrmEdge;
    SDL_RenderCopy(renderer, tileTexture, &ogRect, &attackRect);

    // Hiss
    ogRect.x = calcX(hissFrame);
    ogRect.y = calcY(hissFrame);
    attackRect.x = SCREEN_W - size * 2 - dstFrmEdge;
    attackRect.y = dstFrmEdge;
    SDL_RenderCopy(renderer, tileTexture, &ogRect, &attackRect);

    /*******************
     ** RENDER ITEMS  **
     ** BOTTOM MIDDLE **
     *******************/

    gap = 16, size = 64, dstFrmEdge = 8;
    // choose to render full or empty item box if item is collected
    int itemBoxToRender[4] = {
        beak ? ITEM_BEAKER : ITEM_EMPTY,
        therm ? ITEM_THERMOMETER : ITEM_EMPTY,
        plant ? ITEM_PLANT : ITEM_EMPTY,
        vial ? ITEM_VIAL : ITEM_EMPTY};
    for (i = 0; i < 4; i++)
    {
        ogRect.x = calcX(itemBoxToRender[i]);
        ogRect.y = calcY(itemBoxToRender[i]);
        // equally space and center item boxes
        j = i - 2;
        xOffset = j * (size + gap) + dstFrmEdge;
        dstRect.x = SCREEN_W / 2 + xOffset;
        dstRect.y = SCREEN_H - size - dstFrmEdge;
        SDL_RenderCopy(renderer, tileTexture, &ogRect, &dstRect);
    }
}

/**
 * @brief Calculate the current top left coords for the portion of the map displayed on the screen
 * based on the location of the cat. Cat should stay as close to the center of the screen as possible.
 * Updates the mapTopLeft array held privately in View.
 *
 * @param catX cat's x-location on map
 * @param catY cat's y-location on map
 * @param mapWidth number of map columns
 * @param mapHeight number of map rows
 */
void View::calcTopLeft(int catX, int catY, int mapWidth, int mapHeight)
{
    int tilesX = SCREEN_W / tileSize;
    int tilesY = SCREEN_H / tileSize;

    // set upper left x coordinate
    if (catX - (tilesX / 2) <= 0) // handle edge case on left
        mapTopLeft[0] = 0;
    else if (catX + (tilesX / 2) >= mapWidth) // handle edge case on right
        mapTopLeft[0] = mapWidth - tilesX;
    else
        mapTopLeft[0] = catX - (tilesX / 2);

    // set upper left y coordinate
    if (catY - (tilesY / 2) <= 0) // handle edge case on top
        mapTopLeft[1] = 0;
    else if (catY + (tilesY / 2) >= mapHeight) // handle edge case on bottom
        mapTopLeft[1] = mapHeight - tilesY;
    else
        mapTopLeft[1] = catY - (tilesY / 2);
}

void View::renderDialogBox(std::string dialog)
{   
    int dialogBoxX = 80;
    int dialogBoxY = 450;
    int dialogBoxW = 800;
    int dialogBoxH = 175;
    int infoMsgW = 350;
    int spacing = 10; // 5 px for boundary considerations

    if (dialog == "")
        return;

    // render box
    SDL_Rect dialogBox = {dialogBoxX, dialogBoxY, dialogBoxW, dialogBoxH};

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128)) // gray box
        csci437_error("RenderDrawColor error");

    if (SDL_RenderFillRect(renderer, &dialogBox))
        csci437_error("RenderFillRect error");

    SDL_RenderDrawRect(renderer, &dialogBox);

    // render text
    SDL_Color color = {255, 255, 255}; // white
    SDL_Surface *text = TTF_RenderText_Blended_Wrapped(font, dialog.c_str(), color, 600); // 600 = px of dialog text before spilling to next line
    if (text == NULL)
        csci437_error("Unable to render text!");

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, text);
    if (textTexture == NULL)
        csci437_error("Could not create texture from surface!");

    // info text to inform the player how to get to next dialog
    const char *textInfo = "press i to continue, p to skip";
    SDL_Color infoColor = {170, 240, 209}; // teal blue
    SDL_Surface *infoText = TTF_RenderText_Blended_Wrapped(font, textInfo, infoColor, infoMsgW); // 300 = x of dialog text before spilling to next line
    if (infoText == NULL)
        csci437_error("Unable to render text!");

    SDL_Texture *infoTextTexture = SDL_CreateTextureFromSurface(renderer, infoText);
    if (infoTextTexture == NULL)
        csci437_error("Could not create texture from surface!");

    SDL_Rect dialogRect = {dialogBoxX + spacing, dialogBoxY + spacing, text->w, text->h};
    SDL_Rect infoRect = {dialogBoxX + dialogBoxW - spacing - infoText->w, dialogBoxY + dialogBoxH - spacing - infoText->h, infoText->w, infoText->h};

    SDL_RenderCopy(renderer, textTexture, NULL, &dialogRect);
    SDL_RenderCopy(renderer, infoTextTexture, NULL, &infoRect);
}

/**
 * @brief Every screen will be rendered in the same rectangle in the window.
 *
 * @param pngX location of the specific screen on the screens png
 * @param pngY location of the specific screen on the screens png
 */
void View::renderScreen(int pngX, int pngY)
{
    screenRectOg.x = pngX;
    screenRectOg.y = pngY;

    SDL_RenderCopy(renderer, screenTexture, &screenRectOg, &screenRectDst);
}

/**
 * @brief Iterate through the screen's button array to render all buttons
 *
 * @param screen
 * @param mouseX
 * @param mouseY
 */
void View::renderButtons(std::shared_ptr<InfoScreen> screen, int mouseX, int mouseY)
{
    // all buttons are the same size. x and y will be handled next
    SDL_Rect ogButton = {0, 0, screen->getButtonWidth(), screen->getButtonHeight()};  // png
    SDL_Rect dstButton = {0, 0, screen->getButtonWidth(), screen->getButtonHeight()}; // window destination

    std::shared_ptr<InfoScreen::Button> curButton;
    for (int i = 0; i < screen->getNumOfButtons(); i++)
    {
        curButton = screen->getButtonByIndex(i);
        ogButton.x = curButton->getPNGX();
        ogButton.y = curButton->getPNGY();
        // determine if button is being hovered over and render differently depending
        if (buttonKeyOverlap(curButton, mouseX, mouseY))
            ogButton.x += screen->getButtonWidth();

        dstButton.x = curButton->getScreenX();
        dstButton.y = curButton->getScreenY();
        SDL_RenderCopy(renderer, screenTexture, &ogButton, &dstButton);
    }
}

/**
 * @brief Based on mouse coords, determine if mouse overlaps with the given button
 *
 * @param button to check
 * @param mouseX
 * @param mouseY
 * @return true mouse overlaps
 * @return false mouse does not overlap
 */
bool View::buttonKeyOverlap(std::shared_ptr<InfoScreen::Button> button, int mouseX, int mouseY)
{
    bool xOverlap = mouseX > button->getScreenX() && mouseX < button->getScreenX() + button->getWidth();
    bool yOverlap = mouseY > button->getScreenY() && mouseY < button->getScreenY() + button->getHeight();
    return xOverlap && yOverlap;
}

/**
 * @brief Render start screen and handle start screen user input
 * Also referred to as "title screen"
 *
 * @param logic user input impacts logic
 * @param deltaMs time elapsed since last call in ms
 * @return true game is still running
 * @return false quit game
 */
bool View::handleStartScreen(std::shared_ptr<Logic> logic, int deltaMs)
{
    // Handle events on queue
    SDL_Event e;
    SDL_Rect dstRocket = (SDL_Rect){64, 46, titleScreen->getRocketWidth(), titleScreen->getRocketHeight()}; // rocket destination on screen
    SDL_Rect ogRocket;                                                                                      // rocket location on screen
    bool fadeIn = titleScreen->update(deltaMs);                                                             // update animation for the title screen and determine if fade in is still happening

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    while (SDL_PollEvent(&e) != 0)
    {
        // User requests quit
        if (e.type == SDL_QUIT)
            return false;

        // User presses a key
        if (e.type == SDL_KEYDOWN)
        {
            // quit
            if (e.key.keysym.sym == SDLK_q)
            {
                return false;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            // play button
            if (buttonKeyOverlap(titleScreen->getButtonByIndex(0), mouseX, mouseY))
            {
                logic->setIsPlaying(true);
                return true;
            }
            // quit button
            if (buttonKeyOverlap(titleScreen->getButtonByIndex(1), mouseX, mouseY))
            {
                return false;
            }
        }
    }

    SDL_RenderClear(renderer);
    renderScreen(titleScreen->getPNGX(), titleScreen->getPNGY());
    if (!fadeIn) // don't render buttons until screen animation is complete
    {
        renderButtons(titleScreen, mouseX, mouseY);
    }
    ogRocket = (SDL_Rect){titleScreen->getRocketPNGX(), titleScreen->getRocketPNGY(), titleScreen->getRocketWidth(), titleScreen->getRocketHeight()}; // rocket location on png changes with animation
    SDL_RenderCopy(renderer, screenRocketTexture, &ogRocket, &dstRocket);
    SDL_RenderPresent(renderer);

    return true;
}

/**
 * @brief Render end screen and handle end screen user input
 * Handles both if owner died and if owner lived
 *
 * @param logic user input impacts logic
 * @param deltaMs time elapsed since last call in ms
 * @return true game is still running
 * @return false quit game
 */
bool View::handleEndScreen(std::shared_ptr<Logic> logic, int deltaMs)
{
    // Handle events on queue
    //std::cout<<"end screen";
    SDL_Event e;
    std::shared_ptr<InfoScreen> screen = winScreen;
    /**if (logic->getBossDead()) // determine which screen to use
    {
        screen = ownerDieScreen;
    }**/

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    while (SDL_PollEvent(&e) != 0)
    {
        // User requests quit
        if (e.type == SDL_QUIT)
            return false;

        // User presses a key
        if (e.type == SDL_KEYDOWN)
        {
            // quit
            if (e.key.keysym.sym == SDLK_q)
            {
                return false;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            // replay button
            if (buttonKeyOverlap(screen->getButtonByIndex(0), mouseX, mouseY))
            {
                logic->setIsTutorialCutscene(true);
                cutsceneManager->setCutsceneId(0);
                logic->playAgain();
                return true;
            }
            // quit button
            if (buttonKeyOverlap(screen->getButtonByIndex(1), mouseX, mouseY))
            {
                return false;
            }
        }
    }

    SDL_RenderClear(renderer);
    renderScreen(screen->getPNGX(), screen->getPNGY());
    renderButtons(screen, mouseX, mouseY);
    SDL_RenderPresent(renderer);
    return true;
}
/**
 * @brief Render pause screen and handle pause screen user input
 *
 * @param logic user input impacts logic
 * @param deltaMs time elapsed since last call in ms
 * @return true game is still running
 * @return false quit game
 */
bool View::handlePauseScreen(std::shared_ptr<Logic> logic, int deltaMs)
{
    // Handle events on queue
    SDL_Event e;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    while (SDL_PollEvent(&e) != 0)
    {
        // User requests quit
        if (e.type == SDL_QUIT)
            return false;

        // User presses a key
        if (e.type == SDL_KEYDOWN)
        {
            // quit
            if (e.key.keysym.sym == SDLK_q)
            {
                return false;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            // resume button
            if (buttonKeyOverlap(pauseScreen->getButtonByIndex(0), mouseX, mouseY))
            {
                logic->setIsPaused(false);
                logic->setIsPlaying(true);
                return true;
            }
            // reload button
            if (buttonKeyOverlap(pauseScreen->getButtonByIndex(1), mouseX, mouseY))
            {
                logic->reloadGame();
                std::cout << "reload game\n";
                logic->setIsPaused(false);
                logic->setIsPlaying(true);
                return true;
            }
            // quit button
            if (buttonKeyOverlap(pauseScreen->getButtonByIndex(2), mouseX, mouseY))
            {
                return false;
            }
        }
    }
    SDL_RenderClear(renderer);
    renderScreen(pauseScreen->getPNGX(), pauseScreen->getPNGY());
    renderButtons(pauseScreen, mouseX, mouseY);
    SDL_RenderPresent(renderer);
    return true;
}

bool View::handleDeadScreen(std::shared_ptr<Logic> logic, int deltaMs)
{
    // Handle events on queue
    SDL_Event e;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    while (SDL_PollEvent(&e) != 0)
    {
        // User requests quit
        if (e.type == SDL_QUIT)
            return false;

        // User presses a key
        if (e.type == SDL_KEYDOWN)
        {
            // quit
            if (e.key.keysym.sym == SDLK_q)
            {
                return false;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            // reload button
            if (buttonKeyOverlap(pauseScreen->getButtonByIndex(0), mouseX, mouseY))
            {
                logic->handleCatDeath();
                logic->setIsPlaying(true);
                return true;
            }
            // quit button
            if (buttonKeyOverlap(pauseScreen->getButtonByIndex(1), mouseX, mouseY))
            {
                return false;
            }
        }
    }

    SDL_RenderClear(renderer);
    renderScreen(dieScreen->getPNGX(), dieScreen->getPNGY());
    renderButtons(dieScreen, mouseX, mouseY);
    SDL_RenderPresent(renderer);
    return true;
}

/**
 * @brief Update the View side of the game
 * Consider user input's effect on logic
 * Render map and objects
 * Determine camera for part of map shown
 *
 * @param logic game logic
 * @param deltaMs change in time since last game loop in milliseconds
 * @return true game continues
 * @return false game ends (quit)
 */
bool View::update(std::shared_ptr<Logic> logic, int deltaMs) //
{
    // update the frame ratio used to determine the animation frame
    animationManager->update(deltaMs);
    calcTopLeft(logic->getCat()->getX(), logic->getCat()->getY(), logic->getMap()->getMapX(), logic->getMap()->getMapY());

    SDL_RenderClear(renderer);

    renderMap(logic->getMap(), deltaMs);
    renderDialogBox(talk);
    renderInfoBar(logic->getCat(), logic->getGameStateIndex(0), // vial
                  logic->getGameStateIndex(1),                  // thermometer
                  logic->getGameStateIndex(2),                  // plant
                  logic->getGameStateIndex(3));                 // beaker

    SDL_RenderPresent(renderer);
    SDL_Event e;

    if (logic->isCutscene())
    {
        talk = cutsceneManager->handleCutscene(logic, deltaMs, e);
        if (cutsceneManager->getIsQuit())
            return false;
        isPaused = cutsceneManager->getIsPaused();
    }
    else
    {
        // Handle events on queue
        //  Handle Input
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_q)
                return false;

            // User presses a key
            if (e.type == SDL_KEYDOWN)
            {
                /** QUIT **/
                if (e.key.keysym.sym == SDLK_q)
                    return false;

                /** MOVE UP **/
                if ((e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) && isPaused == false)
                    logic->getCat()->catMove(logic->getMap(), Tile::Direction::UP, deltaMs);
                /** MOVE DOWN **/
                if ((e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) && isPaused == false)
                    logic->getCat()->catMove(logic->getMap(), Tile::Direction::DOWN, deltaMs);
                /** MOVE RIGHT **/
                if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) && isPaused == false)
                    logic->getCat()->catMove(logic->getMap(), Tile::Direction::RIGHT, deltaMs);
                /** MOVE LEFT **/
                if ((e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) && isPaused == false)
                    logic->getCat()->catMove(logic->getMap(), Tile::Direction::LEFT, deltaMs);

                /** ITEM INTERACTION **/
                if (e.key.keysym.sym == SDLK_i && logic->itemAdjacent())
                    logic->itemInteract();
                /** SCIENTIST INTERACTION **/
                if (e.key.keysym.sym == SDLK_i && logic->scientistAdjacent())
                {
                    isPaused = true;
                    talk = logic->scientistInteract();
                    if (talk == "")
                        isPaused = false;
                }


                /** SCRATCH ATTACK **/
                if (e.key.keysym.sym == SDLK_e)
                    bool enemyDied = logic->getCat()->scratch(logic->getMap(), deltaMs);
                /** HISS ATTACK **/
                if (e.key.keysym.sym == SDLK_r)
                    logic->getCat()->hiss(logic->getMap(), deltaMs);

                // temporary until game conditions to for hard, soft, and puzzle reset
                if (e.key.keysym.sym == SDLK_b)
                {
                    logic->saveGame();
                }
                /**if (e.key.keysym.sym == SDLK_n)
                {
                    // tests both resetGame and soft reset(load save)
                    logic->handleCatDeath();
                }**/
                /**if (e.key.keysym.sym == SDLK_m)
                {
                    logic->resetPuzzle(1); // change number to test specific puzzles later
                }**/
                /**if (e.key.keysym.sym == SDLK_h)
                {
                    logic->getCat()->heal(1);
                }**/
                /**if (e.key.keysym.sym == SDLK_j)
                {
                    logic->setBossBool(true);
                    logic->setIsPlaying(false);
                    for (int i = 0; i < 4; i++)
                    {
                        if (logic->getGameStateIndex(i) != 1)
                        {
                            logic->setGameStateIndex(1, i);
                            int stateNum = logic->getGameStateIndex(i);
                        }
                    }
                }**/
                /**if (e.key.keysym.sym ==SDLK_o) {
                    logic->setIsFinal(1);
                }**/
                /** PAUSE THE GAME **/
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    logic->setIsPaused(true);
                    logic->setIsPlaying(false);
                }
            }
        }

        
    }
    return true;
}
