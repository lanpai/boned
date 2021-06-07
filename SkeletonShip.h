#pragma once

#include "Skeleton.h"
#include "Graphics.h"

class Game;

class SkeletonShip : public Skeleton {
    public:
        SkeletonShip(Game* game, int difficulty);

        void Loop();
        void Input(char input);

        bool PlaceShip(int x, int y, bool isVertical, char type, char** board);
        bool Attack(int x, int y, char** board);

        int CheckWinCon();
        int GetWinCon();
        void SkeletonSetup();
        void SkeletonTurn();

        void DrawBoard(int x, int y);
        void DrawKey(int x, int y);

        string GetCursorPos(int x = -1, int y = -1);
        string GetShipName(char type);

    private:
        Game* game;
        int difficulty;
        int turn = -1;

        char** boardSkeleton;
        char** boardPlayer;

        int width, height;

        int cursorX, cursorY;
        bool isVertical = false;
        char currentShip = 'C';

        Sprite skeletonClose;
        Sprite skeleton;
};
