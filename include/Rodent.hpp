#ifndef RODENT_HPP
#define RODENT_HPP
#include "Mobile.hpp"
#include "Immobile.hpp"
#include "Cat.hpp"
#include <set>
#include <cmath>

class Cat;

class Rodent : public Mobile
{
private:
    static constexpr double BITE_COOLDOWN_TIME = 4.0;
    static constexpr double AGGRO_COOLDOWN_TIME = 5.0;
    static constexpr double MOVE_COOLDOWN_TIME = 1.0;

    int health;
    double aggro;
    double stunned;
    double biteCooldown;
    bool home;
    int homePos[2];
    int lastCatPos[2];
    double moveCooldown;

    // Differs for Owner and Rodent
    int maxHP;
    int biteDamage;

    ////////////////////
    // PATHFINDING    //
    // DATA & METHODS //
    ////////////////////

    /**
     * @brief Node object for each tile to determine the correct path
     * for the rodent using a greedy graph algorithm
     *
     */
    struct Node
    {
        int x;
        int y;
        /** distance to end node */
        int dist;
        /** direction from initial node */
        Direction dir;

        Node(int x, int y, Direction dir) : x(x), y(y), dir(dir), dist(0) {};

        /**
         * @brief distance from node to end node
         * 
         */
        int calcDist(std::shared_ptr<Node> compare)
        {
            return abs(x - compare->x) + abs(y - compare->y);
        };

        /**
         * @brief overload the < operator for set comparison
         */
        bool operator<(const std::shared_ptr<Node> compare) const
        {
            return this->dist <= compare->dist;
        };

        bool isAdjacent(std::shared_ptr<Node> compare) const
        {
            bool yAdj = y == compare->y && (x == compare->x - 1 || x == compare->x + 1);
            bool xAdj = x == compare->x && (y == compare->y - 1 || y == compare->y + 1);
            return yAdj || xAdj;
        };
    };

    std::set<std::shared_ptr<Node>> openSet;   // nodes not yet encountered

    bool findPath(std::shared_ptr<Map> map, int goalX, int goalY, int time);
    void addNodeToSet(int x, int y, Direction dir, std::shared_ptr<Node> endNode);
    bool seesCat(std::shared_ptr<Map> map, std::shared_ptr<Cat> cat);    
    bool sightBlocked(std::shared_ptr<Map> map, int x, int y);
    bool isFacing(int targetX, int targetY);
    bool bite(std::shared_ptr<Cat> cat);
    bool goHome(std::shared_ptr<Map> map, int time);
    bool chaseCat(int lastCatX, int lastCatY, std::shared_ptr<Map> map, int time);
    bool rodentMove(std::shared_ptr<Map> map, int time, bool cat);

public:
    Rodent(int tileId, int uniqueId, int x, int y, int size);

    bool handleCollision(std::shared_ptr<Map> map, int time, int i, int j) override;

    void resetRodent();

    void forceBite();
    bool takeDamage(int damage);

    int getHealth() const { return health; };
    double isAggro() const { return aggro; };
    double isStunned() const { return stunned; };
    int getBiteDamage() const { return biteDamage; };
    double getBiteCooldown() const { return biteCooldown; };
    int getHomeX() const { return homePos[0]; };
    int getHomeY() const { return homePos[1]; };
    bool isHome() const { return home; };
    int getLastCatX() const { return lastCatPos[0]; };
    int getLastCatY() const { return lastCatPos[1]; };
    double getMoveCooldown() const { return moveCooldown; };

    void setHealth(int health) { this->health = health; };
    void setAggro(double isAggro) { this->aggro = isAggro; };
    void setStunned(double isStunned) { this->stunned = isStunned; };
    void setBiteDamage(int biteDamage) { this->biteDamage = biteDamage; };
    void setBiteCooldown(double biteCooldown) { this->biteCooldown = biteCooldown; };
    void setHomePos(int x, int y)
    {
        this->homePos[0] = x;
        this->homePos[1] = y;
    };
    void setLastCatPos(int x, int y)
    {
        this->lastCatPos[0] = x;
        this->lastCatPos[1] = y;
    };
    void setHome(bool home) { this->home = home; };
    void setMoveCooldown(double moveCooldown) { this->moveCooldown = moveCooldown; };

    bool isConfined(std::shared_ptr<Map> map);

    void update(std::shared_ptr<Map> map, std::shared_ptr<Cat> cat, int deltaMs);
    void ownerUpdate(std::shared_ptr<Map> map, std::shared_ptr<Cat> cat, int deltaMs);
    bool forceRodentMove(std::shared_ptr<Map> map, Direction dir, int time);
};

#endif