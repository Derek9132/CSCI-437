#include "AnimationManager.hpp"

/**
 * @brief Construct a new Animation Manager:: Animation Manager object
 * Animation Manager stores all animations using the Animations struct.
 * This constructor initializes the Animation structs and inserts them into the Animation array.
 */
AnimationManager::AnimationManager(void)
{
    // ALL SPOTS IN ARRAY MUST BE FILLED TO PREVENT SEG FAULTS

    ////////////////////////
    //// CAT ANIMATIONS ////
    ////////////////////////

    std::shared_ptr<Animation> catIdle = std::make_shared<Animation>(171); // cat idle down will act as placeholder
    std::shared_ptr<Animation> catPet = std::make_shared<Animation>(219);  // only one direction for cat pet, will be reused
    std::shared_ptr<Animation> catDie = std::make_shared<Animation>(222);  // only one direction for cat die, will be reused

    // up direction
    animationArray[0][0][0] = std::make_shared<Animation>(183); // idle
    animationArray[0][1][0] = std::make_shared<Animation>(186); // move
    animationArray[0][2][0] = std::make_shared<Animation>(189); // attack
    animationArray[0][3][0] = std::make_shared<Animation>(192); // hiss
    animationArray[0][4][0] = catIdle;                          // pounce
    animationArray[0][5][0] = catDie;                           // die
    animationArray[0][6][0] = catPet;                           // pet

    // down direction //
    animationArray[0][0][1] = catIdle;                          // idle
    animationArray[0][1][1] = std::make_shared<Animation>(174); // move
    animationArray[0][2][1] = std::make_shared<Animation>(177); // attack
    animationArray[0][3][1] = std::make_shared<Animation>(180); // hiss
    animationArray[0][4][1] = catIdle;                          // pounce
    animationArray[0][5][1] = catDie;                           // die
    animationArray[0][6][1] = catPet;                           // pet

    // left direction //
    animationArray[0][0][2] = std::make_shared<Animation>(207); // idle
    animationArray[0][1][2] = std::make_shared<Animation>(210); // move
    animationArray[0][2][2] = std::make_shared<Animation>(213); // attack
    animationArray[0][3][2] = std::make_shared<Animation>(216); // hiss
    animationArray[0][4][2] = catIdle;                          // pounce
    animationArray[0][5][2] = catDie;                           // die
    animationArray[0][6][2] = catPet;                           // pet

    // right direction //
    animationArray[0][0][3] = std::make_shared<Animation>(195); // idle
    animationArray[0][1][3] = std::make_shared<Animation>(198); // move
    animationArray[0][2][3] = std::make_shared<Animation>(201); // attack
    animationArray[0][3][3] = std::make_shared<Animation>(204); // hiss
    animationArray[0][4][3] = catIdle;                          // pounce
    animationArray[0][5][3] = catDie;                           // die
    animationArray[0][6][3] = catPet;                           // pet

    ///////////////////////
    // RODENT ANIMATIONS //
    ///////////////////////

    std::shared_ptr<Animation> rodentDie = std::make_shared<Animation>(124);      // reuse die
    std::shared_ptr<Animation> rodentIdleDown = std::make_shared<Animation>(91);  // same idle for all directions
    std::shared_ptr<Animation> rodentAggroIdle = std::make_shared<Animation>(97); // same aggro idle for all directions denoted by pounce

    // up direction //
    std::shared_ptr<Animation> rodentMoveUp = std::make_shared<Animation>(100);
    animationArray[1][0][0] = rodentIdleDown;
    animationArray[1][1][0] = rodentMoveUp;                     // move
    animationArray[1][2][0] = std::make_shared<Animation>(118); // attack/bite
    animationArray[1][3][0] = rodentMoveUp;                     // move and hiss (aka agressive) are the same bc rodent faces away
    animationArray[1][4][0] = rodentAggroIdle;                  // no pounce - idle placeholder
    animationArray[1][5][0] = rodentDie;                        // die
    animationArray[1][6][0] = rodentIdleDown;                   // no pet - idle placeholder

    // down direction //
    animationArray[1][0][1] = rodentIdleDown;                   // idle
    animationArray[1][1][1] = std::make_shared<Animation>(94);  // move
    animationArray[1][2][1] = std::make_shared<Animation>(115); // attack
    animationArray[1][3][1] = rodentAggroIdle;                  // hiss = aggressive
    animationArray[1][4][1] = rodentAggroIdle;                  // no pounce - idle
    animationArray[1][5][1] = rodentDie;                        // die
    animationArray[1][6][1] = rodentIdleDown;                   // no pet - idle instead

    // left direction and right direction (the same for rodents) //

    std::shared_ptr<Animation> rodentMoveSide = std::make_shared<Animation>(106);

    // left direction //
    animationArray[1][0][2] = rodentIdleDown;                   // idle
    animationArray[1][1][2] = std::make_shared<Animation>(109); // move
    animationArray[1][2][2] = std::make_shared<Animation>(124); // attack
    animationArray[1][3][2] = std::make_shared<Animation>(112); // hiss = aggressive
    animationArray[1][4][2] = rodentAggroIdle;                  // no pounce
    animationArray[1][5][2] = rodentDie;                        // die
    animationArray[1][6][2] = rodentIdleDown;                   // no pet

    // right direction //
    animationArray[1][0][3] = rodentIdleDown;                   // idle
    animationArray[1][1][3] = std::make_shared<Animation>(103); // move
    animationArray[1][2][3] = std::make_shared<Animation>(121); // attack
    animationArray[1][3][3] = std::make_shared<Animation>(106); // hiss = aggressive
    animationArray[1][4][3] = rodentAggroIdle;                  // no pounce
    animationArray[1][5][3] = rodentDie;                        // die
    animationArray[1][6][3] = rodentIdleDown;                   // no pet

    // NOTE: png includes animation for aggro and walking, but currently there is no way to represent that

    //////////////////////
    // OWNER ANIMATIONS //
    //////////////////////

    std::shared_ptr<Animation> OwnerIdleHealthy = std::make_shared<Animation>(131);
    std::shared_ptr<Animation> OwnerIdleSick = std::make_shared<Animation>(134);
    std::shared_ptr<Animation> OwnerAggroIdle = std::make_shared<Animation>(136); // infected facing forward move
    OwnerIdleSick->numberOfFrames = 2;

    // up direction //
    std::shared_ptr<Animation> OwnerIdleUp;
    animationArray[2][0][0] = OwnerIdleHealthy;                 // idle
    animationArray[2][1][0] = OwnerIdleHealthy;                 // move = healthy move
    animationArray[2][2][0] = std::make_shared<Animation>(153); // attack
    animationArray[2][3][0] = std::make_shared<Animation>(139); // hiss = aggro move
    animationArray[2][4][0] = OwnerAggroIdle;                   // pounce = aggro idle aka front aggro move
    animationArray[2][5][0] = OwnerIdleSick;                    // die = sick
    animationArray[2][6][0] = OwnerIdleHealthy;                 // no pet = idle

    // down direction //
    animationArray[2][0][1] = OwnerIdleHealthy;                 // idle healthy
    animationArray[2][1][1] = OwnerIdleHealthy;                 // move
    animationArray[2][2][1] = std::make_shared<Animation>(140); // attack
    animationArray[2][4][1] = OwnerAggroIdle;                   // hiss = aggro move
    animationArray[2][5][1] = OwnerAggroIdle;                   // pounce = aggro idle aka front aggro move
    animationArray[2][3][1] = OwnerIdleSick;                    // die = sick
    animationArray[2][6][1] = OwnerIdleHealthy;                 // no pet = idle

    // left direction //
    animationArray[2][0][2] = OwnerIdleHealthy;                 // idle healthy
    animationArray[2][1][2] = OwnerIdleHealthy;                 // move
    animationArray[2][2][2] = std::make_shared<Animation>(156); // attack
    animationArray[2][3][2] = std::make_shared<Animation>(142); // hiss = aggro move
    animationArray[2][3][2]->numberOfFrames = 4;
    animationArray[2][4][2] = OwnerAggroIdle;                   // pounce = aggro idle
    animationArray[2][5][2] = OwnerIdleSick;                    // die = sick
    animationArray[2][6][2] = OwnerIdleHealthy;                 // no pet = idle

    // right direction //
    animationArray[2][0][3] = OwnerIdleHealthy;                 // idle healthy
    animationArray[2][1][3] = OwnerIdleHealthy;                 // move
    animationArray[2][2][3] = std::make_shared<Animation>(159); // attack
    animationArray[2][3][3] = std::make_shared<Animation>(146); // hiss = aggro move
    animationArray[2][3][3]->numberOfFrames = 3;
    animationArray[2][4][3] = OwnerAggroIdle;                   // pounce = aggro idle
    animationArray[2][5][3] = OwnerIdleSick;                    // die = sick
    animationArray[2][6][3] = OwnerIdleHealthy;                 // no pet = idle

    //////////////////////////
    // SCIENTIST ANIMATIONS //
    //////////////////////////
    // Refer to Scientist.hpp for scientist conversion chart

    // png locations:
    int derekLoc = 51;
    int sebLoc = 61;
    int colleenLoc = 71;
    int janelleLoc = 81;

    // Scientist 1 -- Seb -- Up //
    std::shared_ptr<Animation> seb = std::make_shared<Animation>(sebLoc);
    for (int i = 0; i < 7; i++) {
        animationArray[3][i][0] = seb;
    }

    // Scientist 2 -- Janelle -- Down //
    std::shared_ptr<Animation> janelle = std::make_shared<Animation>(janelleLoc);
    for (int i = 0; i < 7; i++) {
        animationArray[3][i][1] = janelle;
    }

    // Scientist 3 -- Colleen -- Left //
    std::shared_ptr<Animation> colleen = std::make_shared<Animation>(colleenLoc);
    for (int i = 0; i < 7; i++) {
        animationArray[3][i][2] = colleen;
    }

    // Scientist 4 -- Derek -- Right //
    std::shared_ptr<Animation> derek = std::make_shared<Animation>(derekLoc);
    for (int i = 0; i < 7; i++) {
        animationArray[3][i][3] = derek;
    }

    /////////////////////
    // ITEM ANIMATIONS //
    /////////////////////

    int pngLoc = 41; // png locations
    for (int item = 0; item < 4; item++)
    {
        std::shared_ptr<Animation> saved = std::make_shared<Animation>(pngLoc);
        saved->numberOfFrames = 1; // only 1 frames for items for now
        for (int i = 0; i < 7; i++)
        {
            animationArray[4][i][item] = saved;
        }
        pngLoc += 1; // move to next sprite on png for next item
    }
}

/**
 * @brief returns the png tileId for the next animation frame
 * based on the tile type, animation state, and direction
 *
 * @param tile object that contains type, state, and direction information
 * @return int the png ID that corresponds to tile location on the tileset png
 */
int AnimationManager::playAnimation(const std::shared_ptr<Tile> tile, double frameRatio = -1.0)
{

    // enum values from tile object are indexes for the array to find the corresponding animation object to the oject state
    int type = tile->getType() - 3; // subtract 3 to match enum value with animation array
    if (type < 0)
        type = 0; // address possible out of bound errors with negative numbers
    int state = tile->getState();
    int dir = tile->getDirection();
    double tempFrameRatio = this->frameRatio;

    // if cat moving, use the cat frame ratio instead of the basic animation ratio
    // this ensures that the player sees immediate feedback from moving the cat
    if (frameRatio != -1.0 && state != 0) // if idle, use animation manager ratio because the cat and rodent frame ratios are too fast
    {
        tempFrameRatio = frameRatio;
    }

    // find animation in animationArray
    std::shared_ptr<Animation> animationObj = animationArray[type][state][dir];

    int startingFrame = animationObj->startingFrame;   // this is the first location of the animation on the png
    int numberOfFrames = animationObj->numberOfFrames; // this is the number of frames in the animation

    int frameToRender = startingFrame; // will be a new location on the png based on the starting frame incremented
    frameToRender += findNextFrameIncrement(tempFrameRatio, numberOfFrames);

    return frameToRender;
}

/**
 * determines the number of frames from the initial animation frame
 * based on the current frameRatio and number of frames
 *
 * @param ratio is a number between 0.0 and 1.0 that represents the progression through animation frames
 * @param numberOfFrames
 * @return int the amount to increment the starting frame to find the current animation frame
 */
int AnimationManager::findNextFrameIncrement(double ratio, int numberOfFrames)
{
    // std::cout << ratio << "ratio, number of frames" << numberOfFrames << "\n";
    switch (numberOfFrames)
    {
    case 2:
    {
        // 1.0 divided by 2
        if (ratio > 0.5 && ratio < 1.0)
            return 1;
        break;
    }
    case 3:
    {
        // 1.0 divided by 3
        if (ratio > 0.33 && ratio <= 0.66)
            return 1;
        if (ratio > 0.66 && ratio <= 0.99)
            return 2;
        break;
    }
    case 4:
    {
        // 1.0 divided by 4
        if (ratio > 0.25 && ratio <= 0.50)
            return 1;
        if (ratio > 0.50 && ratio <= 0.75)
            return 2;
        if (ratio > 0.75 && ratio < 1.0)
            return 3;
        break;
    }
    case 6:
    {
        // 1.0 divided by 6
        if (ratio > 0.16 && ratio <= 0.33)
            return 1;
        if (ratio > 0.33 && ratio <= 0.50)
            return 2;
        if (ratio > 0.50 && ratio <= 0.66)
            return 3;
        if (ratio > 0.66 && ratio <= 0.83)
            return 4;
        if (ratio > 0.83 && ratio < 1.0)
            return 5;
    }
    case 10:
    {
        // 1.0 divided by 10
        if (ratio > 0.1 && ratio <= 0.2)
            return 1;
        if (ratio > 0.2 && ratio <= 0.3)
            return 2;
        if (ratio > 0.3 && ratio <= 0.4)
            return 3;
        if (ratio > 0.4 && ratio <= 0.5)
            return 4;
        if (ratio > 0.5 && ratio <= 0.6)
            return 5;
        if (ratio > 0.6 && ratio <= 0.7)
            return 6;
        if (ratio > 0.7 && ratio <= 0.8)
            return 7;
        if (ratio > 0.8 && ratio <= 0.9)
            return 8;
        if (ratio > 0.9 && ratio < 1)
            return 9;
        break;
    }
    }
    return 0; // no increment
}

/**
 * @brief updates the frameRatio, which is used to progress animation frames based on elapsed time
 *
 * @param deltaMs the change in time elapsed in milliseconds
 */
void AnimationManager::update(int deltaMs)
{
    // frameRatio should always be between 0.0 and 1.0. when it exceeds 1, it should be reset
    if (frameRatio >= 1)
    {
        frameRatio = 0;
    }
    else
    {
        frameRatio += deltaMs / 1000.0 * speed;
    }
}
