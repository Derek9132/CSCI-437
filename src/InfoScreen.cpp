/**
 * @file InfoScreen.cpp
 * @author Janelle Krupicka
 * @brief used to display the start, pause, and end screens.
 * Methods and variabls all in header file
 *
 */

#include "InfoScreen.hpp"

/**
 * @brief Construct a new Info Screen:: Info Screen object
 * 
 * @param screenPNGX location of the specific kind of screen's image on the screens PNG
 * @param screenPNGY location of the specific kind of screen's image on the screens PNG
 * @param buttonPNGX location of the first button on the specific screen on the screens PNG
 * @param buttonPNGY location of the first button on the specific screen on the screens PNG
 * @param pause true if pause screen, which means 3 buttons instead of 2
 */
InfoScreen::InfoScreen(int screenPNGX, int screenPNGY, int buttonPNGX, int buttonPNGY, bool pause)
    : pngX(screenPNGX), pngY(screenPNGY)
{
    // location of the first button on the screen
    int buttonScreenX = 637;
    int buttonScreenY = 341;

    // quit button should be the last button in the array. typically screens should have 2 items, hence index 1, but pause has 3, hence the variable
    int quitButtonI = 1;

    if (pause)
    {
        numOfButtons = 3;
        buttonScreenY = 274; // move buttons up to accomodate a 3rd button
        quitButtonI = 2;     // quit button should be on the bottom of the button stack
    }
    else
        numOfButtons = 2;

    // button one supplied by parameters to the constructor
    buttonArray[0] = std::make_shared<Button>(buttonPNGX, buttonPNGY, buttonScreenX, buttonScreenY);
    if (pause)
    {
        buttonScreenY += buttonSpace;                                                              // next button location on screen
        buttonArray[1] = std::make_shared<Button>(buttonPNGX, 1862, buttonScreenX, buttonScreenY); // reload button is unique to pause
    }
    buttonScreenY += buttonSpace; // next button location on screen

    buttonArray[quitButtonI] = std::make_shared<Button>(buttonPNGX, quitPNGY, buttonScreenX, buttonScreenY);
}

/**
 * @brief Checks the buttonArray stored by the screen for a button
 * Note: buttons will be rendered in order with the last button being quit
 * 
 * @param i index of the button in the array
 * @return std::shared_ptr<InfoScreen::Button>
 */
std::shared_ptr<InfoScreen::Button> InfoScreen::getButtonByIndex(int i)
{
    if (i >= numOfButtons) // prevent out of bound errors
        return buttonArray[0];
    return buttonArray[i];
}