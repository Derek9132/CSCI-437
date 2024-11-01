/**
 * @file TitleScreen.cpp
 * @author Janelle Krupicka
 * @brief Title screen object for rendering in View
 * Inherits from InfoScreen. Includes animation information and
 * rocket information for rendering.
 * 
 */

#include "TitleScreen.hpp"

/**
 * @brief Construct a new Title Screen:: Title Screen object
 * 
 * @param screenPNGX locaton of the title screen on screens.png
 * @param screenPNGY location of the title screen on screens.png
 * @param buttonPNGX location of the top button on screens.png
 * @param buttonPNGY location of the top button on screens.png
 */
TitleScreen::TitleScreen(int screenPNGX, int screenPNGY, int buttonPNGX, int buttonPNGY)
    : InfoScreen(screenPNGX, screenPNGY, buttonPNGX, buttonPNGY, false)
{
    setSpeed(0.0000000000001);

    // construct a rocket object to render (consists of preset values)
    rocket = std::make_shared<Rocket>();
    rocket->animationSpeed = 0.00005;
}

/**
 * @brief Update the animation ratios for the screen fade in and rocket animation
 * based on the time elapsed.
 * 
 * @param deltaMs time elapsed in milliseconds
 * @return true screen fade in is continuing, don't render buttons yet
 * @return false screen fade in has stopped, render buttons
 */
bool TitleScreen::update(int deltaMs)
{
    // update rocket fire animation
    // 3 animation frames for rocket fire
    // rocket y does not change, x moves across png
    //std::cout << deltaMs / 1000.0 << "\n";
    setRocketRatio(getRocketRatio() + getRocketSpeed() * (deltaMs / 1000.0));
    //setRocketRatio(getRocketRatio() + 0.01);
    if (getRocketRatio() >= 1.0)
    {
        setRocketRatio(0.0);
    }
    if (getRocketRatio() <= .33)
    {
        setRocketPNGX(0);
    }
    else if (getRocketRatio() <= .66)
    {
        setRocketPNGX(getRocketWidth());
    }
    else
    {
        setRocketPNGX(getRocketWidth() * 2);
    }

    // update screen fade in
    // if animation is done the pngY will be less than zero, stop subtracting
    if (getPNGY() - (deltaMs / 1000.0) * speed <= 0) // y starts at 1408 for title screen and moves towards 0
    {
        setPNGY(0);
        return false;
    }
    setPNGY(getPNGY() - (deltaMs / 1000.0) * speed);
    return true;
}