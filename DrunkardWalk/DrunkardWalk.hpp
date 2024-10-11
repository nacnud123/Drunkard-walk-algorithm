#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <Bits.h>

class DrunkWalk
{
private:
    enum class gridSpace { empty, floor, wall };

    struct Vector2
    {
        int X, Y;

        Vector2 operator+=(const Vector2& other)
        {
            X += other.X;
            Y += other.Y;
            return *this;
        }
        Vector2 operator-=(const Vector2& other)
        {
            Y -= other.Y;
            X -= other.X;
            return *this;
        }
    };

    struct walker
    {
        Vector2 dir;
        Vector2 pos;
    };

    gridSpace** grid;
    int roomHeight, roomWidth;
    Vector2 worldSize = { 100, 100 };

    std::vector<walker> walkers;

    float chanceWalkerChangeDir = 0.5f, chanceWalkerSpawn = 0.05f, chanceWalkerDestroy = 0.05f;
    int maxWalkers = 30;
    float percentToFill = 0.2f;

    void setup();
    void createFloors();
    void createWalls();
    void removeSingleWalls();
    void printDungeon();
    Vector2 randomDirection();
    int numberOfFloors();
public:
    DrunkWalk(const int widthIn, const int heightIn, const int walkersIn);
    ~DrunkWalk();
};