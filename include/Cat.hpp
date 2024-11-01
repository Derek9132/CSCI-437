#ifndef CAT_HPP
#define CAT_HPP
#include "Mobile.hpp"
#include "Rodent.hpp"

class Rodent;

class Cat : public Mobile
{
private:
    static const int MAX_HP = 18;
    static const int SCRATCH_DAMAGE = 6;
    static const int HISS_RAD = 3;
    static constexpr double HISS_STUN_TIME = 4.0;
    static const int POUNCE_DISTANCE = 5;

    int health;
    double scratchCooldown;
    int scratchAmmo;
    double hissCooldown;

protected:
    bool handleCollision(std::shared_ptr<Map> map, int time, int i, int j) override;
    std::shared_ptr<Rodent> getTarget(std::shared_ptr<Map> map, Direction dir, int i, int j);

public:
    static constexpr double SCRATCH_COOLDOWN_TIME = 3.0;
    static constexpr double HISS_COOLDOWN_TIME = 10.0;
    static constexpr double POUNCE_COOLDOWN_TIME = 20.0;

    Cat(int tileId, int uniqueId, int x, int y, int size);

    int getHealth() const { return health; };
    double getScratchCooldown() const { return scratchCooldown; };
    int getScratchAmmo() const { return scratchAmmo; };
    double getHissCooldown() const { return hissCooldown; };

    void setHealth(int health) { this->health = health; };
    void setScratchCooldown(double scratchCooldown) { this->scratchCooldown = scratchCooldown; };
    void setScratchAmmo(int scratchAmmo) { this->scratchAmmo = scratchAmmo; };
    void setHissCooldown(double hissCooldown) { this->hissCooldown = hissCooldown; };

    void update(std::shared_ptr<Map> map, int deltaMs) override;
    bool scratch(std::shared_ptr<Map> map, int deltaMs);
    void forceScratch();
    void forcePet();
    void hiss(std::shared_ptr<Map> map, int deltaMs);
    void heal(int hp);
    bool takeDamage(int damage);
    void resetCat();

    /** use as a wrapper for Mobile::move to handle cat moveCooldown */
    bool catMove(std::shared_ptr<Map> map, Direction dir, int time);
};

#endif