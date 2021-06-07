#include "Map.h"

#include <cstdlib>
#include <vector>
#include <limits.h>
#include <cmath>
#include "Game.h"
#include "Graphics.h"
#include "SkeletonTTT.h"
#include "SkeletonC4.h"
#include "SkeletonShip.h"
#include "SkeletonChess.h"

Map::Map(Game* game, int difficulty)
    : game(game)
    , difficulty(difficulty) {
    GenerateMap();
    GenerateSkeletons();

    Int2 playerPos = FindRandomEmpty();
    playerX = playerPos.x;
    playerY = playerPos.y;
    map[playerY][playerX] = '@';
}

Map::~Map() {
    for (int i = 0; i < skeletons.size(); i++)
        delete skeletons[i];
}

void Map::Loop() {
    DrawMap(1, 1, 98, 40);

    switch (difficulty) {
        case 0:
            Graphics::DrawText("Difficulty: EASY", 1, 1);
            break;
        case 1:
            Graphics::DrawText("Difficulty: MEDIUM", 1, 1);
            break;
        case 2:
            Graphics::DrawText("Difficulty: HARD", 1, 1);
            break;
    }

    // Render the controls box
    string controls = "move [u]p   move [l]eft   move [d]own   move [r]ight";
    Graphics::DrawText(controls, 1, 48);
}

void Map::Input(char input) {
    switch (input) {
        case 'u':
            MovePlayer(0,-1);
            break;
        case 'l':
            MovePlayer(-1,0);
            break;
        case 'd':
            MovePlayer(0,1);
            break;
        case 'r':
            MovePlayer(1,0);
            break;
    }
    MoveSkeletons();
}

void Map::MovePlayer(int deltaX, int deltaY) {
    if (map[playerY + deltaY][playerX + deltaX] == '.') {
        map[playerY][playerX] = '.';
        playerX += deltaX;
        playerY += deltaY;
        map[playerY][playerX] = '@';
    }
    else {
        game->Log("There's something blocking your path...");
    }
}

void GenBreadthFirstMap(int map[MAP_WIDTH][MAP_HEIGHT], int x, int y) {
    bool adjusted;
    int i = 0;
    do {
        adjusted = false;
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_HEIGHT; x++) {
                if (map[y][x] == i) {
                    if (y > 0 && map[y-1][x] == -1) {
                        map[y-1][x] = i+1;
                        adjusted = true;
                    }
                    if (x > 0 && map[y][x-1] == -1) {
                        map[y][x-1] = i+1;
                        adjusted = true;
                    }
                    if (y < MAP_HEIGHT - 1 && map[y+1][x] == -1) {
                        map[y+1][x] = i+1;
                        adjusted = true;
                    }
                    if (x < MAP_WIDTH - 1 && map[y][x+1] == -1) {
                        map[y][x+1] = i+1;
                        adjusted = true;
                    }
                }
            }
        }
        i++;
    } while (adjusted);
}

void Map::MoveSkeletons() {
    for (int i = 0; i < skeletons.size(); i++) {
        int x = skeletons[i]->GetX();
        int y = skeletons[i]->GetY();
        int newX = x;
        int newY = y;

        if (rand() % 3 > 0) { // 33% change to move inoptimally
            // Generate optimal pathing
            int breadthFirstMap[MAP_HEIGHT][MAP_WIDTH];
            for (int y = 0; y < MAP_HEIGHT; y++)
                for (int x = 0; x < MAP_WIDTH; x++)
                    if (map[y][x] != ' ')
                        breadthFirstMap[y][x] = -1;
                    else
                        breadthFirstMap[y][x] = INT_MAX;
            breadthFirstMap[playerY][playerX] = 0;
            GenBreadthFirstMap(breadthFirstMap, playerX, playerY);

            int current = breadthFirstMap[y][x];
            if (y > 0 && breadthFirstMap[y-1][x] < current)
                newY--;
            else if (x > 0 && breadthFirstMap[y][x-1] < current)
                newX--;
            else if (y < MAP_HEIGHT - 1 && breadthFirstMap[y+1][x] < current)
                newY++;
            else if (x < MAP_WIDTH - 1 && breadthFirstMap[y][x+1] < current)
                newX++;
        }
        else {
            // Generate random pathing
            switch (rand() % 4) {
                case 0:
                    newY--;
                    break;
                case 1:
                    newX--;
                    break;
                case 2:
                    newY++;
                    break;
                case 3:
                    newX++;
                    break;
            }
        }

        // Check collision
        if (map[newY][newX] == '.') {
            map[y][x] = '.';
            map[newY][newX] = 'S';
            skeletons[i]->SetPos(newX, newY);
        }

        // Check battle condition
        if (pow(newX-playerX, 2) + pow(newY-playerY, 2) <= 1) { // Squared distance
            game->To(skeletons[i]);
        }
    }
}

void Map::DrawMap(int x1, int y1, int x2, int y2) {
    int centerX = (x1 + x2) / 2;
    int centerY = (y1 + y2) / 2;
    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            int deltaX = x - centerX;
            int deltaY = y - centerY;

            if ((playerX + deltaX >= 0 && playerX + deltaX < MAP_WIDTH) &&
                (playerY + deltaY >= 0 && playerY + deltaY < MAP_HEIGHT))
                Graphics::DrawPixel(map[playerY + deltaY][playerX + deltaX], x, y);
        }
    }
}

void Map::GenerateMap() {
    // Random walker algorithm

    int maxTunnels = 70;
    int maxLength = 8;

    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = ' ';
        }

    int x, y;
    x = rand() % (MAP_WIDTH - 2) + 1;
    y = rand() % (MAP_HEIGHT - 2) + 1;

    Int2 directions[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };
    Int2 lastDirection = {0,0};
    Int2 randDirection;

    while (maxTunnels) {
        do {
            randDirection = directions[rand() % 4];
        } while (
            (randDirection.x == -lastDirection.x && randDirection.y == -lastDirection.y) ||
            (randDirection.x == lastDirection.x && randDirection.y == lastDirection.y));

        int randLength = rand() % (maxLength + 1);
        int tunnLength = 0;

        while (tunnLength < randLength) {
            if ((y == 1 && randDirection.y == -1) ||
                (x == 1 && randDirection.x == -1) ||
                (y == MAP_HEIGHT - 2 && randDirection.y == 1) ||
                (x == MAP_WIDTH - 2 && randDirection.x == 1))
                break;
            map[y][x] = '.';
            x += randDirection.x;
            y += randDirection.y;
            tunnLength++;
        }

        if (tunnLength) {
            lastDirection = randDirection;
            maxTunnels--;
        }
    }
}

void Map::GenerateSkeleton(Skeleton* skeleton) {
    Int2 pos = FindRandomEmpty();
    skeletons.push_back(skeleton);
    skeletons.back()->SetPos(pos.x, pos.y);
    map[pos.y][pos.x] = 'S';
}

void Map::GenerateSkeletons() {
    GenerateSkeleton(new SkeletonTTT(game, difficulty));
    GenerateSkeleton(new SkeletonC4(game, difficulty));
    GenerateSkeleton(new SkeletonShip(game, difficulty));
    GenerateSkeleton(new SkeletonChess(game, difficulty));
}

void Map::DelSkeleton(Skeleton* skeleton) {
    for (int i = 0; i < skeletons.size(); i++)
        if (skeleton == skeletons[i]) {
            map[skeletons[i]->GetY()][skeletons[i]->GetX()] = '.';
            delete skeletons[i];
            skeletons.erase(skeletons.begin() + i);
        }
    if (skeletons.size() == 0)
        game->Restart(1);
}

Int2 Map::FindRandomEmpty() {
    int floorCount = 0;
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            if (map[y][x] == '.')
                floorCount++;

    int floor = rand() % floorCount;
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            if (map[y][x] == '.')
                if (floor-- == 0) {
                    return Int2{ x,y };
                }

    return Int2{ -1,-1 };
}
