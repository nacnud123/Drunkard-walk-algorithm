#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <Bits.h>
#include "DrunkardWalk.hpp"

DrunkWalk::DrunkWalk(const int widthIn, const int heightIn, const int walkersIn) // Constructor / run the program
{
    // Assign the vars
    worldSize.X = widthIn;
    worldSize.Y = heightIn;
    maxWalkers = walkersIn;

    // Run the program.
    setup();
    createFloors();
    createWalls();
    removeSingleWalls();
    printDungeon();
}

DrunkWalk::~DrunkWalk() // Destructor
{
    for (int i = 0; i < roomWidth; ++i)
    {
        delete[] grid[i];
    }
    delete[] grid;
}

void DrunkWalk::setup() // Set up the grid and the world
{
    roomHeight = worldSize.Y;
    roomWidth = worldSize.X;

    grid = new gridSpace * [roomWidth];

    for (int i = 0; i < roomWidth; ++i)
    {
        grid[i] = new gridSpace[roomHeight];
    }

    for (int x = 0; x < roomWidth - 1; x++)
    {
        for (int y = 0; y < roomHeight - 1; y++)
        {
            grid[x][y] = gridSpace::empty;
        }
    }

    walker newWalker;
    newWalker.dir = randomDirection();

    Vector2 spawnPos = { roomWidth / 2, roomHeight / 2 };
    newWalker.pos = spawnPos;
    walkers.push_back(newWalker);
}

void DrunkWalk::createFloors()
{
    srand((unsigned)time(NULL)); // Init random scene
    int iterations = 0;
    do
    {
        // Create floor at the position of every walker
        for (const walker& myWalker : walkers)
        {
            grid[myWalker.pos.X][myWalker.pos.Y] = gridSpace::floor;
        }

        // Chance: Destroy walker
        int numberChecks = walkers.size();
        for (int i = 0; i < numberChecks; i++)
        {
            if (static_cast<float>(rand()) / RAND_MAX < chanceWalkerDestroy && walkers.size() > 1)
            {
                walkers.erase(walkers.begin() + i);
                break;
            }
        }

        // Change: Walker pick new direction
        for (int i = 0; i < walkers.size(); i++)
        {
            if (static_cast<float>(rand()) / RAND_MAX < chanceWalkerChangeDir)
            {
                walkers[i].dir = randomDirection();
            }
        }

        // Change: Spawn new walker
        numberChecks = walkers.size();
        for (int i = 0; i < numberChecks; i++)
        {
            if (static_cast<float>(rand()) / RAND_MAX < chanceWalkerSpawn && walkers.size() < maxWalkers)
            {
                walker newWalker;
                newWalker.dir = randomDirection();
                newWalker.pos = walkers[i].pos;
                walkers.push_back(newWalker);
            }
        }

        // Move Walker
        for (int i = 0; i < walkers.size(); i++)
        {
            walkers[i].pos += walkers[i].dir;
        }

        // Avoid boarder of grid
        for (int i = 0; i < walkers.size(); i++)
        {
            walker thisWalker = walkers[i];

            thisWalker.pos.X = std::clamp(walkers[i].pos.X, 1, roomWidth - 2);
            thisWalker.pos.Y = std::clamp(walkers[i].pos.Y, 1, roomHeight - 2);

            walkers[i] = thisWalker;
        }

        // Check to exit loop
        if (static_cast<float>(numberOfFloors()) / (roomWidth * roomHeight) > percentToFill)
        {
            break;
        }
        iterations++;
    } while (iterations < 100000);
}

void DrunkWalk::createWalls()
{
    // Loop through every grid space
    for (int x = 0; x < roomWidth - 1; x++)
    {
        for (int y = 0; y < roomHeight - 1; y++)
        {
            // If theres a floor, check the space around it
            if (grid[x][y] == gridSpace::floor)
            {
                // If any surrounding spaces are empty place a wall.
                if (grid[x][y + 1] == gridSpace::empty)
                {
                    grid[x][y + 1] = gridSpace::wall;
                }
                if (grid[x][y - 1] == gridSpace::empty)
                {
                    grid[x][y - 1] = gridSpace::wall;
                }
                if (grid[x + 1][y] == gridSpace::empty)
                {
                    grid[x + 1][y] = gridSpace::wall;
                }
                if (grid[x - 1][y] == gridSpace::empty)
                {
                    grid[x - 1][y] = gridSpace::wall;
                }
            }
        }
    }
}

void DrunkWalk::removeSingleWalls()
{
    for (int x = 0; x < roomWidth - 1; x++)
    {
        for (int y = 0; y < roomHeight - 1; y++)
        {
            // If there a wall, check space around it
            if (grid[x][y] == gridSpace::wall)
            {
                // Assume all space around wall are floors
                bool allFloors = true;
                // Check each side to see if they are all floors
                for (int checkX = -1; checkX <= 1; checkX++)
                {
                    for (int checkY = -1; checkY <= 1; checkY++)
                    {
                        if (x + checkX < 0 || x + checkX > roomWidth - 1 ||
                            y + checkY < 0 || y + checkY > roomHeight - 1)
                        {
                            // Skip checks that are out of range
                            continue;
                        }

                        if ((checkX != 0 && checkY != 0) || (checkX == 0 && checkY == 0))
                        {
                            // Skip corners and center
                            continue;
                        }

                        if (grid[x + checkX][y + checkY] != gridSpace::floor)
                        {
                            allFloors = false;
                        }
                    }
                }
                if (allFloors)
                {
                    grid[x][y] = gridSpace::floor;
                }
            }
        }
    }
}

void DrunkWalk::printDungeon() // Print the dungeon
{
    for (int x = 0; x < roomWidth; x++)
    {
        for (int y = 0; y < roomHeight; y++)
        {
            switch (grid[x][y])
            {
            case gridSpace::empty:
                std::cout << ' ';
                break;
            case gridSpace::floor:
                std::cout << '.';
                break;
            case gridSpace::wall:
                std::cout << '#';
                break;
            }
        }
        std::cout << std::endl;
    }
}

DrunkWalk::Vector2 DrunkWalk::randomDirection()
{

    int choice = rand() % 4;
    switch (choice)
    {
    case 0: return { 0, -1 };
    case 1: return { -1, 0 };
    case 2: return { 0, 1 };
    default: return { 1, 0 };
    }
}

int DrunkWalk::numberOfFloors()
{
    int count = 0;
    for (int x = 0; x < roomWidth; x++)
    {
        for (int y = 0; y < roomHeight; y++)
        {
            if (grid[x][y] == gridSpace::floor)
            {
                count++;
            }
        }
    }
    return count;
}