#pragma once

#include <vector>
#include "Skeleton.h"

using namespace std;

#define MAP_WIDTH 75
#define MAP_HEIGHT 75

class Game;

class Map : public Screen {
    public:
        Map(Game* game, int difficulty);
        ~Map();

        void Loop();
        void Input(char input);

        void MovePlayer(int deltaX, int deltaY);
        void MoveSkeletons();
        void DrawMap(int x1, int y1, int x2, int y2);
        void GenerateMap();
        void GenerateSkeleton(Skeleton* skeleton);
        void GenerateSkeletons();
        void DelSkeleton(Skeleton* skeleton);
        Int2 FindRandomEmpty();

    private:
        Game* game;
        int difficulty;
        char map[MAP_HEIGHT][MAP_WIDTH];

        int playerX, playerY;
        vector<Skeleton*> skeletons;
};
