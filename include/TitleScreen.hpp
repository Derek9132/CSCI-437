#ifndef TITLESCREEN_HPP
#define TITLESCREEN_HPP
#include "InfoScreen.hpp"
#include <memory>

class TitleScreen : public InfoScreen
{
private:
    struct Rocket
    {
        int pngX = 0;
        int pngY = 0;
        const int width = 289;
        const int height = 604;
        double animationRatio = 0.0;
        double animationSpeed = 1.0;
    };

    std::shared_ptr<Rocket> rocket;
    double speed;
    double getRocketRatio() const { return rocket->animationRatio; };
    double getRocketSpeed() const { return rocket->animationSpeed; };
    void setRocketRatio(double ratio) { this->rocket->animationRatio = ratio; };
    void setSpeed(double speed) { this->speed = speed; };
    double getSpeed() const { return speed; };
    void setRocketPNGX(int pngX) { this->rocket->pngX = pngX; };

public:
    TitleScreen(int screenPNGX, int screenPNGY, int buttonPNGX, int buttonPNGY);

    bool update(int deltaMs);
    int getRocketPNGX() const { return rocket->pngX; };
    int getRocketPNGY() const { return rocket->pngY; };
    int getRocketWidth() const { return rocket->width; };
    int getRocketHeight() const { return rocket->height; };
};

#endif