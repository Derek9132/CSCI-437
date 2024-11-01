#ifndef INFOSCREEN_HPP
#define INFOSCREEN_HPP
#include <iostream>
#include <memory>

class InfoScreen
{
public:
    struct Button
    {
        int pngX, pngY;
        int screenX, screenY;
        bool hover = false;
        const int width = 247;  // button width in pixels - also acts as the change in x to get to cursor hover button
        const int height = 113; // button height in pixels
        Button(int pngX, int pngY, int x, int y) : pngX(pngX), pngY(pngY), screenX(x), screenY(y) {}
        int getPNGX() const { return pngX; };
        int getPNGY() const { return pngY; };
        int getScreenX() const { return screenX; };
        int getScreenY() const { return screenY; };
        int getHover() const { return hover; };
        int getWidth() const { return width; };
        int getHeight() const { return height; };
        void setHover(bool hover) { this->hover = hover; };
    };

private:
    int pngX, pngY; // location of static screen on screens.png
    const int width = 960;
    const int height = 704;
    // location of button on screen stored in InfoScreen
    // because buttons will be in the same location for every screen
    const int buttonX = 637;
    const int buttonY = 341; // increment this by button space to get the next y location
    const int buttonSpace = 141;
    int numOfButtons; // 2 or 3
    const int quitPNGX = 1920;
    const int quitPNGY = 1523;

    std::shared_ptr<Button> buttonArray[3];
    // std::shared_ptr<Button> button1; // changes based on type of InfoScreen

protected:
public:
    InfoScreen(int screenPNGX, int screenPNGY, int buttonPNGX, int buttonPNGY, bool pause);

    int getPNGX() const { return pngX; };
    int getPNGY() const { return pngY; };
    int getWidth() const { return width; };
    int getHeight() const { return width; };
    int getButtonWidth() const { return buttonArray[0]->width; };
    int getButtonHeight() const { return buttonArray[0]->height; };
    int getNumOfButtons() const { return numOfButtons; };
    std::shared_ptr<Button> getButtonByIndex(int i);

    int getButtonX() const { return buttonX; };
    int getButtonY() const { return buttonY; };
    /** Top button plus the space between the buttons to get the y location of the lower button */
    int getQuitButtonY() const { return buttonY + buttonSpace; };

    void setPNGX(int pngX) { this->pngX = pngX; };
    void setPNGY(int pngY) { this->pngY = pngY; };
};

#endif